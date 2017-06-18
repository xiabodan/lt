/* V4L2 video picture grabber
   Copyright (C) 2009 Mauro Carvalho Chehab <mchehab@infradead.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <libv4l2.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <omp.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#define FRAME_WIDTH  1280
#define FRAME_HEIGHT 720

#define H_STRIDE            1920
#define H_ACTIVE            1920
#define V_ACTIVE            1080

#define XFILTER_REG_AP_CTRL        0x00
#define XFILTER_REG_GIE            0x04
#define XFILTER_REG_IER            0x08
#define XFILTER_REG_ISR            0x0c
#define XFILTER_REG_ROWS	       0x14
#define XFILTER_REG_COLS    	   0x1c

#define BUFFER_OFFSETS		0x00870000
#define FILTER_VDMA_BADDR   0x43040000
#define FILTER_BASEADDR     0x69000000
#define IMG_BASEADDR        0x1F500000
#define FILTER_OUT_BASEADDR 0x1FA00000

#define DEBUG_MODE

#ifdef DEBUG_MODE
	#define DEBUG_Text(fmt) printf(fmt)
	#define DEBUG_Printf(fmt,...) printf(fmt,__VA_ARGS__)
#else
	#define DEBUG_Text(fmt)
	#define DEBUG_Printf(fmt,...)
#endif

#define IN0()	DEBUG_Printf(">> IN >> %s",__func__)
#define OUT0()	DEBUG_Printf("<< OUT << %s",__func__)


#define REG_WRITE(addr, off, val) (*(volatile unsigned int*)(addr+off)=(val))
#define REG_READ(addr,off) (*(volatile unsigned int*)(addr+off))

struct buffer {
	void   *start;
	size_t length;
};

void __attribute__ ((noinline)) neonMemCopy_gas(unsigned char* src, unsigned char* dst, int num_bytes);
void __attribute__ ((noinline)) neonRGBtoRGBA_gas(unsigned char* src, unsigned char* dst, int numPix);
static void xioctl(int fh, int request, void *arg);
void configureVDMA(unsigned long vdma_physical_base);
void stopVDMA(unsigned long vdma_physical_base);
void initSobel(unsigned long sobel_physical_base);
void startSobel(unsigned long sobel_physical_base);
void copy_rgb24todev(CvMat *img, unsigned long in_buffer);
void copy_devtorgb24(CvMat *img, unsigned long in_buffer);
void copy_mem2dev(uint8_t *img, unsigned long in_buffer);
void copy_dev2mem(uint8_t *img, unsigned long in_buffer);

int main(int argc, char **argv)
{
	struct v4l2_format	      fmt;
	struct v4l2_buffer	      buf;
	struct v4l2_requestbuffers      req;
	enum v4l2_buf_type	      type;
	fd_set			  fds;
	struct timeval		  tv;
	int			     r, fd = -1;
	unsigned int		    i, n_buffers;
	char			    *dev_name = "/dev/video0";
	char			    out_name[256];
	struct buffer		   *buffers;
	
	int sw_flag=0,hw_flag=0,hw_flag_old = 0;
	float sw_tmr, get_frame_tmr, or_tmr;
	float tmr_hw_copy_devtorgb24 = 0;
	float tmr_hw_conv_back = 0;
	float tmr_hw_copy_dev2mem = 0;
	float tmr_hw_calc = 0;
	float tmr_hw_copy_rgb24todev = 0;
	float tmr_hw_copy_mem2dev = 0;
	float tmr_hw_conv	 = 0;
	float tmr_hw_total	 = 0;
	float tmr_hw_loop	 = 0;
	float tmr_hw_release = 0;
	float tmr_rest = 0;
	
	float sum_sw_tmr = 0, sum_get_frame_tmr = 0, sum_or_tmr = 0;
	float sum_tmr_hw_copy_devtorgb24 = 0;
	float sum_tmr_hw_conv_back = 0;
	float sum_tmr_hw_copy_dev2mem = 0;
	float sum_tmr_hw_calc = 0;
	float sum_tmr_hw_copy_rgb24todev = 0;
	float sum_tmr_hw_copy_mem2dev = 0;
	float sum_tmr_hw_conv	= 0;
	float sum_tmr_hw_total = 0;
	float sum_tmr_hw_loop = 0;
	float sum_tmr_hw_release = 0;
	float sum_tmr_rest = 0;
	
	int orCount = 0;
	int gfCount = 0;
	int swCount = 0;
	int hwCount = 0;

	fd = v4l2_open(dev_name, O_RDWR | O_NONBLOCK, 0);
	if (fd < 0) {
		perror("Cannot open device");
		exit(EXIT_FAILURE);
	}

	CLEAR(fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = FRAME_WIDTH;
	fmt.fmt.pix.height      = FRAME_HEIGHT;
	//int pixelformat = V4L2_PIX_FMT_GREY;// gray scale
	int pixelformat = V4L2_PIX_FMT_RGB24;// V4L2_PIX_FMT_RGB32 doesn't work
	fmt.fmt.pix.pixelformat = pixelformat; //yuyv converted with neon to rgb32
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	printf("fmt create success\n");
	xioctl(fd, VIDIOC_S_FMT, &fmt);
	if (fmt.fmt.pix.pixelformat != pixelformat) {
		printf("Libv4l didn't accept the chosen format. Can't proceed.\n");
		exit(EXIT_FAILURE);
	}
	if ((fmt.fmt.pix.width != FRAME_WIDTH) || (fmt.fmt.pix.height != FRAME_HEIGHT))
		printf("Warning: driver is sending image at %dx%d\n",
			fmt.fmt.pix.width, fmt.fmt.pix.height);
	printf("fmt success\n");

	CLEAR(req);
	req.count = 2;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	xioctl(fd, VIDIOC_REQBUFS, &req);
	printf("buffer request success\n");

	buffers = calloc(req.count, sizeof(*buffers));
	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		CLEAR(buf);

		buf.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = n_buffers;

		xioctl(fd, VIDIOC_QUERYBUF, &buf);

		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start = v4l2_mmap(NULL, buf.length,
			      PROT_READ | PROT_WRITE, MAP_SHARED,
			      fd, buf.m.offset);

		if (MAP_FAILED == buffers[n_buffers].start) {
			perror("mmap");
			exit(EXIT_FAILURE);
		}
	}
	printf("buffer allocation success\n");

	for (i = 0; i < n_buffers; ++i) {
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		xioctl(fd, VIDIOC_QBUF, &buf);
	}
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	xioctl(fd, VIDIOC_STREAMON, &type);
	printf("buffer clear success\n");

	initSobel(FILTER_BASEADDR);
	configureVDMA(FILTER_VDMA_BADDR);

	cvNamedWindow("window",CV_WINDOW_AUTOSIZE);

	tmr_rest = omp_get_wtime();
	for (;;) {
		get_frame_tmr = omp_get_wtime();
		do {
			FD_ZERO(&fds);
			FD_SET(fd, &fds);

			/* Timeout. */
			tv.tv_sec = 2;
			tv.tv_usec = 0;

			r = select(fd + 1, &fds, NULL, NULL, &tv);
		} while ((r == -1 && (errno = EINTR)));
		if (r == -1) {
			perror("select");
			return errno;
		}

		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		xioctl(fd, VIDIOC_DQBUF, &buf);
		
		CvMat* rgb = cvCreateMatHeader(fmt.fmt.pix.height, fmt.fmt.pix.width, CV_8UC3);
		cvCreateData(rgb);
		memcpy(rgb->data.ptr, buffers[buf.index].start, buf.bytesused);
		xioctl(fd, VIDIOC_QBUF, &buf);

		sum_get_frame_tmr += omp_get_wtime() - get_frame_tmr;
		gfCount++;
		
		sum_tmr_rest += omp_get_wtime() - tmr_rest;

		if(sw_flag == 0 & hw_flag == 0)
		{
			or_tmr = omp_get_wtime();
			
			// convert to bgr
			CvMat* bgr = cvCreateMat(rgb->rows, rgb->cols, CV_8UC3);
			int from_to[] = { 0,2,  1,1,  2,0 };
			cvMixChannels((const CvArr**)&rgb, 1, (CvArr**)&bgr, 1, from_to, 3);
			
			cvShowImage("window", bgr);
			
			cvReleaseMat(&bgr);

			sum_or_tmr += omp_get_wtime() - or_tmr;
			orCount++;
		}
		else if(sw_flag == 1)
		{
			sw_tmr = omp_get_wtime();
			
			CvMat* sobelFrame = cvCreateMat(rgb->rows, rgb->cols, CV_8UC1);
			CvMat* srcGray = cvCreateMat(rgb->rows, rgb->cols, CV_8UC1);
			CvMat* grad_x = cvCreateMat(rgb->rows, rgb->cols, CV_32FC1);
			CvMat* grad_y = cvCreateMat(rgb->rows, rgb->cols, CV_32FC1);
			CvMat* abs_grad_x = cvCreateMat(rgb->rows, rgb->cols, CV_8UC1);
			CvMat* abs_grad_y = cvCreateMat(rgb->rows, rgb->cols, CV_8UC1);
			
			cvCvtColor(rgb, srcGray, CV_RGB2GRAY);
			//void cvSobel(const CvArr* src, CvArr* dst, int xorder, int yorder, int aperture_size=3 )
			cvSobel(srcGray, grad_x, 1, 0, 3);
			cvConvertScaleAbs(grad_x, abs_grad_x, 1, 0);
			cvSobel(srcGray, grad_y, 0, 1, 3);
			cvConvertScaleAbs(grad_y, abs_grad_y, 1, 0);
			cvAddWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobelFrame);
			
			cvShowImage("window", sobelFrame);
			
			cvReleaseMat(&sobelFrame);
			cvReleaseMat(&srcGray);
			cvReleaseMat(&grad_x);
			cvReleaseMat(&grad_y);
			cvReleaseMat(&abs_grad_x);
			cvReleaseMat(&abs_grad_y);
			
			sum_sw_tmr += omp_get_wtime() - sw_tmr;
			swCount++;
		}
		else if(hw_flag == 1)
		{
			if (hw_flag_old != 1)
			{
				tmr_hw_loop = omp_get_wtime();
			}
			//if(omp_get_wtime() - tmr_hw_loop < 0)
			//printf("omp_get_wtime() - tmr_hw_loop = %d\n", (int)(omp_get_wtime() - tmr_hw_loop));
			//printf("%f, %f \n", omp_get_wtime() - tmr_hw_loop, sum_tmr_hw_loop);
			sum_tmr_hw_loop += omp_get_wtime() - tmr_hw_loop;
			tmr_hw_loop = omp_get_wtime();
			tmr_hw_total = omp_get_wtime();

			tmr_hw_copy_rgb24todev = omp_get_wtime();
			copy_rgb24todev(rgb, IMG_BASEADDR);
			sum_tmr_hw_copy_rgb24todev += omp_get_wtime() - tmr_hw_copy_rgb24todev;
			
			// calculation on accelerator
			tmr_hw_calc = omp_get_wtime();
			startSobel(FILTER_BASEADDR);
			sum_tmr_hw_calc += omp_get_wtime() - tmr_hw_calc;
			
			// copy result back
			CvMat* sobelOut = cvCreateMat(rgb->rows, rgb->cols, CV_8UC4);
			
			tmr_hw_copy_dev2mem = omp_get_wtime();
			copy_dev2mem(sobelOut->data.ptr, FILTER_OUT_BASEADDR);
			sum_tmr_hw_copy_dev2mem += omp_get_wtime() - tmr_hw_copy_dev2mem;

			sum_tmr_hw_total += omp_get_wtime() - tmr_hw_total;
			// show result (4 channel)
			cvShowImage("window", sobelOut);
			
			tmr_hw_release = omp_get_wtime();

			cvReleaseMat(&sobelOut);

			sum_tmr_hw_release += omp_get_wtime() - tmr_hw_release;
			hwCount++;
		}
		
		tmr_rest = omp_get_wtime();

		cvReleaseMat(&rgb);
		
		char c = cvWaitKey(1);
		if ( c == 27 ) break;
		hw_flag_old = hw_flag;
		if ( c == 's' )
		{
			sw_flag = 1;
			hw_flag = 0;
		}
		if ( c == 'h' )
		{
			sw_flag = 0;
			hw_flag = 1;
		}
		if ( c == 'o' )
		{
			sw_flag = 0;
			hw_flag = 0;
		}
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	xioctl(fd, VIDIOC_STREAMOFF, &type);
	for (i = 0; i < n_buffers; ++i)
		v4l2_munmap(buffers[i].start, buffers[i].length);
	v4l2_close(fd);
	
	stopVDMA(FILTER_VDMA_BADDR);

	if(orCount == 0) orCount = 1;
	if(gfCount == 0) gfCount = 1;
	if(swCount == 0) swCount = 1;
	if(hwCount == 0) hwCount = 1;
	
	// print results
	float avg_or_tmr = sum_or_tmr / orCount;
	float avg_gf_tmr = sum_get_frame_tmr / gfCount;
	float avg_sw_tmr = sum_sw_tmr / swCount;
	float avg_tmr_hw_copy_devtorgb24 = sum_tmr_hw_copy_devtorgb24 / hwCount;
	float avg_tmr_hw_conv_back = sum_tmr_hw_conv_back / hwCount;
	float avg_tmr_hw_copy_dev2mem = sum_tmr_hw_copy_dev2mem / hwCount;
	float avg_tmr_hw_calc = sum_tmr_hw_calc / hwCount;
	float avg_tmr_hw_copy_rgb24todev = sum_tmr_hw_copy_rgb24todev / hwCount;
	float avg_tmr_hw_copy_mem2dev = sum_tmr_hw_copy_mem2dev / hwCount;
	float avg_tmr_hw_conv = sum_tmr_hw_conv / hwCount;
	float sum_avg_hw = avg_tmr_hw_copy_devtorgb24 + avg_tmr_hw_conv_back + avg_tmr_hw_copy_dev2mem + avg_tmr_hw_calc + avg_tmr_hw_copy_rgb24todev + avg_tmr_hw_copy_mem2dev + avg_tmr_hw_conv;
	float avg_hw_total = sum_tmr_hw_total / hwCount;
	float avg_hw_loop = sum_tmr_hw_loop / hwCount;
	float avg_tmr_hw_release = sum_tmr_hw_release / hwCount;
	float avg_rest = sum_tmr_rest / gfCount;
	printf("\n");
	printf("Get frame latency with %d counts = %f seconds\n", gfCount, (avg_gf_tmr));
	printf("Original latency with %d counts = %f seconds\n", orCount, (avg_or_tmr));
	printf("Software Filtering latency with %d counts = %f seconds\n", swCount, (avg_sw_tmr));
	printf("\n");
	printf("Hardware Filtering latency with %d counts = %f seconds\n", hwCount, (avg_hw_total));
	printf("Hardware Filtering sum with %d counts = %f seconds\n", hwCount, (sum_avg_hw));
	printf("\n");
	printf("avg_tmr_hw_conv            = %f\n", (avg_tmr_hw_conv));
	printf("avg_tmr_hw_copy_mem2dev    = %f\n", (avg_tmr_hw_copy_mem2dev));
	printf("avg_tmr_hw_copy_rgb24todev = %f\n", (avg_tmr_hw_copy_rgb24todev));
	printf("avg_tmr_hw_calc            = %f\n", (avg_tmr_hw_calc));
	printf("avg_tmr_hw_copy_devtorgb24 = %f\n", (avg_tmr_hw_copy_devtorgb24));
	printf("avg_tmr_hw_copy_dev2mem    = %f\n", (avg_tmr_hw_copy_dev2mem));
	printf("avg_tmr_hw_conv_back       = %f\n", (avg_tmr_hw_conv_back));
	printf("avg_tmr_hw_release         = %f\n", (avg_tmr_hw_release));
	printf("avg_tmr_hw_loop            = %f\n", (avg_hw_loop));
	printf("\navg_tmr_rest               = %f\n", (avg_rest));
	
	
	//printf("Hardware Filtering latency with %d counts = %f seconds\n", hwCount, ((float)avg_hw_tmr)/CLOCKS_PER_SEC);
	//printf("Hardware Filtering latency (w/o memcopy) with %d counts = %f seconds\n", hwCount, ((float)avg_hw_tmr2)/CLOCKS_PER_SEC);
	//printf("Hardware Filtering latency (w/ memcopy and conversion) with %d counts = %f seconds\n", hwCount, ((float)avg_hw_tmr3)/CLOCKS_PER_SEC);

	return 0;
}

static void xioctl(int fh, int request, void *arg)
{
	int r;

	do {
		r = v4l2_ioctl(fh, request, arg);
	} while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

	if (r == -1) {
		fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void configureVDMA(unsigned long vdma_physical_base)
{
	unsigned long int PhysicalAddress = vdma_physical_base ;
	int map_len = 0x20;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* vdma_base_address;

	vdma_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	//check if it worked
	if(vdma_base_address == MAP_FAILED)
	{
		perror("VDMA Mapping memory for absolute memory access failed.\n");
		return;
	}
	DEBUG_Printf ("VDMA mapping 0x%x to 0x%x, size = %d\n", (int )PhysicalAddress, (int)vdma_base_address, map_len );

	REG_WRITE(vdma_base_address, 0x0 /* 0x30 + additional_offset*/, 0x00003);
	REG_WRITE(vdma_base_address,0x58 /*VDMA_FRMDLY_STRIDE */,FRAME_WIDTH*4);
	REG_WRITE(vdma_base_address,0x5c /*VDMA_START_ADDRESS1*/,IMG_BASEADDR);
	//mem_base += BUFFER_OFFSETS;
	REG_WRITE(vdma_base_address,0x60 /*VDMA_START_ADDRESS2*/,IMG_BASEADDR);
	//mem_base += BUFFER_OFFSETS;
	REG_WRITE(vdma_base_address,0x64 /*VDMA_START_ADDRESS3*/,IMG_BASEADDR);

	REG_WRITE(vdma_base_address,0x54/*VDMA_HSIZE*/,FRAME_WIDTH*4);
	REG_WRITE(vdma_base_address,0x50/*VDMA_VSIZE*/,FRAME_HEIGHT);

	REG_WRITE(vdma_base_address, 0x30, 0x00003);
	REG_WRITE(vdma_base_address,0xa8,FRAME_WIDTH*4);
	REG_WRITE(vdma_base_address,0xac,FILTER_OUT_BASEADDR);
	//mem_base += BUFFER_OFFSETS;
	REG_WRITE(vdma_base_address,0xb0,FILTER_OUT_BASEADDR);
	//mem_base += BUFFER_OFFSETS;
	REG_WRITE(vdma_base_address,0xb4,FILTER_OUT_BASEADDR);

	REG_WRITE(vdma_base_address,0xa4,FRAME_WIDTH*4);
	REG_WRITE(vdma_base_address,0xa0,FRAME_HEIGHT);

	munmap((void *)vdma_base_address, map_len);
	close(fd);
}

void stopVDMA(unsigned long vdma_physical_base)
{
	unsigned long int PhysicalAddress = vdma_physical_base ;
	int map_len = 0x20;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* vdma_base_address;

	vdma_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);

	//check if it worked
	if(vdma_base_address == MAP_FAILED)
	{
		perror("VDMA Mapping memory for absolute memory access failed.\n");
		return;
	}

	REG_WRITE(vdma_base_address, 0x0 /* 0x30 + additional_offset*/, 0x00000);
	REG_WRITE(vdma_base_address, 0x30, 0x00000);

	munmap((void *)vdma_base_address, map_len);
	close(fd);
}

void initSobel(unsigned long sobel_physical_base)
{
	unsigned long int PhysicalAddress = sobel_physical_base ;
	int map_len = 0x20;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* sobel_base_address;

	sobel_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);
	//check if it worked
	if(sobel_base_address == MAP_FAILED)
	{
		perror("Sobel Init Mapping memory for absolute memory access failed.\n");
		return;
	}
	DEBUG_Printf ("Sobel mapping 0x%x to 0x%x, size = %d\n", (int )PhysicalAddress, (int)sobel_base_address, map_len );
	REG_WRITE(sobel_base_address,XFILTER_REG_ROWS,FRAME_HEIGHT);
	REG_WRITE(sobel_base_address,XFILTER_REG_COLS,FRAME_WIDTH);
	munmap((void *)sobel_base_address, map_len);
	close(fd);

}

void startSobel(unsigned long sobel_physical_base)
{
	unsigned long int PhysicalAddress = sobel_physical_base ;
	int map_len = 0x20;
	int fd = open( "/dev/mem", O_RDWR);
	unsigned char* sobel_base_address;
	unsigned int done2;

	sobel_base_address = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)PhysicalAddress);
	//check if it worked
	if(sobel_base_address == MAP_FAILED)
	{
		perror("Sobel Start Mapping memory for absolute memory access failed.\n");
		return;
	}
	//DEBUG_Printf ("Sobel mapping 0x%x to 0x%x, size = %d\n ", (int )PhysicalAddress, (int)sobel_base_address, map_len );

	REG_WRITE(sobel_base_address,XFILTER_REG_GIE,1);
    REG_WRITE(sobel_base_address,XFILTER_REG_IER,1);
    REG_WRITE(sobel_base_address,XFILTER_REG_AP_CTRL,1);

	while (1)
	{
	  done2 = REG_READ(sobel_base_address,XFILTER_REG_AP_CTRL);
	  //printf("done value = %d\n\r",done2);
	  if (done2 >> 1 & 0x1)
		  break;
	};

    REG_WRITE(sobel_base_address,XFILTER_REG_GIE,0);
    REG_WRITE(sobel_base_address,XFILTER_REG_AP_CTRL,0);
	munmap((void *)sobel_base_address, map_len);
	close(fd);
}

void copy_rgb24todev(CvMat *img, unsigned long in_buffer)
{
	unsigned long int map_len = FRAME_WIDTH * FRAME_HEIGHT * 4;
	int fd = open("/dev/mem", O_RDWR);

	unsigned char* virtual_addr_in;

	virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)in_buffer);
	if(virtual_addr_in == MAP_FAILED)
	{
		perror("virtual_addr_in mapping for absolute memory access failed.\n");
		return;
	}

	// CvMat* rgb32 = cvCreateMatHeader(img->rows, img->cols, CV_8UC4);
	// cvSetData(rgb32, virtual_addr_in, img->cols * 4);
	// CvMat* ch4 = cvCreateMat(img->rows, img->cols, CV_8UC1);
	// CvArr* in[] = { img, ch4 };
	// int from_to[] = { 0,0,  1,1,  2,2,  3,3 };
	// cvMixChannels((const CvArr**)in, 2, (CvArr**)&rgb32, 1, from_to, 4);
	neonRGBtoRGBA_gas(img->data.ptr, virtual_addr_in, FRAME_WIDTH * FRAME_HEIGHT);
	
	munmap((void *)virtual_addr_in, map_len);
	
	//cvReleaseMat(&rgb32);
	//cvReleaseMat(&ch4);
	
	close(fd);
}

void copy_devtorgb24(CvMat *img, unsigned long in_buffer)
{
	unsigned long int map_len = FRAME_WIDTH * FRAME_HEIGHT * 4;
	int fd = open("/dev/mem", O_RDWR);

	unsigned char* virtual_addr_out;

	virtual_addr_out = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)in_buffer);
	if(virtual_addr_out == MAP_FAILED)
	{
		perror("virtual_addr_in mapping for absolute memory access failed.\n");
		return;
	}

	CvMat* rgb32 = cvCreateMatHeader(img->rows, img->cols, CV_8UC4);
	cvSetData(rgb32, virtual_addr_out, img->cols * 4);

	CvMat* ch4Out = cvCreateMat(img->rows, img->cols, CV_8UC1);
	CvArr* out[] = { img, ch4Out };
	int from_to_out[] = { 0,0,  1,1,  2,2,  3,3 };
	cvMixChannels((const CvArr**)&rgb32, 1, out, 2, from_to_out, 4);
	
	munmap((void *)virtual_addr_out, map_len);

	cvReleaseMat(&ch4Out);
	cvReleaseMat(&rgb32);

	close(fd);
}

void copy_mem2dev(uint8_t *img, unsigned long in_buffer)
{
	unsigned long int map_len = FRAME_WIDTH * FRAME_HEIGHT * 4;
	int fd = open("/dev/mem", O_RDWR);

	unsigned char* virtual_addr_in;

	virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)in_buffer);
	if(virtual_addr_in == MAP_FAILED)
	{
		perror("virtual_addr_in mapping for absolute memory access failed.\n");
		return;
	}
	
	memcpy(virtual_addr_in, img, map_len);

	munmap((void *)virtual_addr_in, map_len);
	close(fd);

}

void copy_dev2mem(uint8_t *img, unsigned long in_buffer)
{
	unsigned long int map_len = FRAME_WIDTH * FRAME_HEIGHT * 4;
	int fd = open("/dev/mem", O_RDWR);

	unsigned char* virtual_addr_out;

	virtual_addr_out = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)in_buffer);
	if(virtual_addr_out == MAP_FAILED)
	{
		perror("virtual_addr_in mapping for absolute memory access failed.\n");
		return;
	}
	
	neonMemCopy_gas(virtual_addr_out, img, map_len);

	munmap((void *)virtual_addr_out, map_len);
	close(fd);

}

void neonPermuteRGBtoBGRA_gas(unsigned char* src, unsigned char* dst, int numPix)
{
	asm(
		"        asr      r3,r2,#31\n"
		"        vmov.i8  d1,#0xff\n"
		"        add      r2,r2,r3,LSR #29\n"
		"        asr      r3,r2,#3\n"
		"        mov      r2,#0\n"
		"        cmp      r3,#0\n"
		"        ble      end\n"
		"loop:\n"
		"        vld3.8   {d4,d5,d6},[r0]!\n"
		"        add      r2,r2,#1\n"
		"        cmp      r3,r2\n"
		"        vmov.f64 d3,d5\n"
		"        vmov.f64 d2,d6\n"
		"        vmov.f64 d5,d1\n"
		"        vmov.f64 d0,d4\n"
		"        vst4.8   {d2,d3,d4,d5},[r1]!\n"
		"        bgt      loop\n"
		"end:\n"
	);
}


void __attribute__ ((noinline)) neonRGBtoRGBA_gas(unsigned char* src, unsigned char* dst, int numPix) 
{ 
	asm( 
		// numpix/8
		"        mov      r2,  r2, lsr #3\n" // numpix/8
		// load alpha channel value
		"        vmov.u8  d3, #0xff\n"
		"loop1:\n"
		// load 8 rgb pixels with deinterleave
		"        vld3.8   {d0,d1,d2}, [r0]!\n"
		// preload next values
		"        pld      [r0,#40]\n"
		"        pld      [r0,#48]\n"
		"        pld      [r0,#56]\n"
		// substract loop counter
		"        subs     r2, r2, #1\n"
		//"        vswp     d0, d2\n"
		// store as 4*8bit values
		"        vst4.8   {d0-d3}, [r1]!\n"
		// loop if not ready
		"        bgt      loop1\n"
	); 
}

void __attribute__ ((noinline)) neonRGBtoGray_gas(unsigned char* src, unsigned char* dst, int numPix) 
{ 
	asm( 
		// numpix/8
		"        mov       r2,  r2, lsr #3\n" 
		// load grayscale multipliers
		"        vmov.u8   d3, #77\n"
		"        vmov.u8   d4, #151\n"
		"        vmov.u8   d5, #28\n"
		"loopgray:\n"
		// load 8 rgb pixels with deinterleave
		"        vld3.8    {d0,d1,d2}, [r0]!\n"
		// preload next values
		"        pld       [r0]\n"
		"        pld       [r0,#8]\n"
		"        pld       [r0,#16]\n" 
		// multiply and add to intermediate value
		"        vmull.u8  q3, d0, d3\n"
		"        vmlal.u8  q3, d1, d4\n"
		"        vmlal.u8  q3, d2, d5\n"
		// shift and round result to 8 bit values
		"        vrshrn.u16 d6, q3, #8\n"
		// substract loop counter
		"        subs      r2, r2, #1\n"
		// store as 8*8bit values with interleave
		"        vst1.8    {d6}, [r1]!\n"
		// loop if not ready
		"        bgt       loopgray\n"
	); 
}


void __attribute__ ((noinline)) neonMemCopy_gas(unsigned char* src, unsigned char* dst, int num_bytes)
{
	// source http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka13544.html
	asm(
		"neoncopypld:\n"
		"		pld 		[r0, #0xC0]\n"
		"		vldm 		r0!,{d0-d7}\n"
    	"		vstm 		r1!,{d0-d7}\n"
    	"		subs 		r2,r2,#0x40\n"
    	"		bge 		neoncopypld\n"
	);
}