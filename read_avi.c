#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
typedef struct RIFF_HEADER{
	uint32_t riff;
	uint32_t filesize;
	uint32_t filetype;
}RIFF_HEADER;

typedef struct AVI_HEADER
{
	uint32_t frame_delay_us;
	uint32_t data_rate_avi;
	uint32_t padding;
	uint32_t parameter_flags;
	uint32_t total_video_frames;
	uint32_t preview_frames;
	uint32_t data_streams;
	uint32_t playback_buffer;
	uint32_t frame_width;
	uint32_t frame_height;
	uint32_t time_scale;
	uint32_t data_rate;		/* frame rate = data rate / time scale*/
	uint32_t starting_time;
	uint32_t size_of_data_chunk;
}AVI_HEADER;

typedef struct _JFIFHeader
{
  uint8_t SOI[2];          /* 00h  Start of Image Marker     */
  uint8_t APP0[2];         /* 02h  Application Use Marker    */
  uint8_t Length[2];       /* 04h  Length of APP0 Field      */
  uint8_t Identifier[5];   /* 06h  "JFIF" (zero terminated) Id String */
  uint8_t Version[2];      /* 07h  JFIF Format Revision      */
  uint8_t Units;           /* 09h  Units used for Resolution */
  uint8_t Xdensity[2];     /* 0Ah  Horizontal Resolution     */
  uint8_t Ydensity[2];     /* 0Ch  Vertical Resolution       */
  uint8_t XThumbnail;      /* 0Eh  Horizontal Pixel Count    */
  uint8_t YThumbnail;      /* 0Fh  Vertical Pixel Count      */
} JFIFHEAD;

int main()
{
	FILE * fp;
	RIFF_HEADER  head;
	AVI_HEADER avi_head;
	uint8_t soi[2];
	
	fp = fopen("out.avi","r");
	
	fseek(fp,32,SEEK_SET);

	fread(&avi_head,1,sizeof(avi_head),fp);

	printf("number of frames: %u\n",avi_head.total_video_frames);
	printf("preview frames: %u\n",avi_head.preview_frames);
	printf("resolution (W x H): %u x %u\n", avi_head.frame_width, avi_head.frame_height);
	printf("size of data chunks: %u\n",avi_head.size_of_data_chunk);
	printf("AVI data rate: %u bps\n",avi_head.data_rate_avi);
	printf("Data Rate: %u\n",avi_head.data_rate);

	FILE * frame;

	char frame_name[100];
	uint8_t bytes[2];
	int bytes_read, frame_count = 0;
	int jpeg_start = 0;
	
	while(1){
		bytes_read =  fread(bytes,1,2,fp);
		if(bytes_read<2){
			break;
		}
		if(bytes[0] == 0xFF && bytes[1]==0xD8){
			jpeg_start=1;
			memset(frame_name,0,100);
			sprintf(frame_name,"Frame%d.jpg",frame_count);
			frame_count++;
			puts(frame_name);
			frame = fopen(frame_name,"wb");
			if(!frame){
				printf("cant open file for reading\n");
			}
			
		}

		if(jpeg_start){
			if(!frame){
				printf("null frame, stop\n");
				continue;
			}
			fwrite(bytes,1,2,frame);
			if(bytes[0]==0xFF && bytes[1]==0xD9)
			{
				jpeg_start = 0;
				fclose(frame);
			}	
		}
	}
}
