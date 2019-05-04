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

int main()
{
	FILE * fp;
	RIFF_HEADER  head;
	AVI_HEADER avi_head;
	
	fp = fopen("out.avi","r");
	
	fseek(fp,32,SEEK_SET);

	fread(&avi_head,1,sizeof(avi_head),fp);

	printf("number of frames: %u\n",avi_head.total_video_frames);
	printf("preview frames: %u\n",avi_head.preview_frames);
	printf("resolution (W x H): %u x %u\n", avi_head.frame_width, avi_head.frame_height);
	printf("size of data chunks: %u\n",avi_head.size_of_data_chunk);
	printf("AVI data rate: %u bps\n",avi_head.data_rate_avi);
	printf("Data Rate: %u\n",avi_head.data_rate);
}
