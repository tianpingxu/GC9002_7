#include "SoundVector.h"
#ifdef PC_SIM_Gen
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"

}
#endif
#define MAX_AUDIO_FRAME_SIZE 192000

#ifdef PC_SIM_Gen
SoundVector::SoundVector(TexBuf *ntexbuf)
{
	texbuf = ntexbuf;
}
#endif

SoundVector::~SoundVector(void)
{
}



void SoundVector::AddSound(u8 SoundID,u8 genSineFreq, u32 StartAddr, u32 size, u8 loop, u32 sineFreq, u32 sineDuration){

	SoundClass newSound;
	newSound.mSoundID = SoundID;
	newSound.genSineFreq = genSineFreq;
	newSound.mStartAddr = StartAddr;
	newSound.mSize = size;
	newSound.loop = loop;
	newSound.sineFreq = sineFreq;
	newSound.sineDuration = sineDuration;
	push_back(newSound);

}

#ifdef PC_SIM_Gen
int SoundVector::generateFromFreq(u32 frequence,u32 audioSampleRate,u32 *Start,u32 *size){
	FILE * fp = fopen(AHMI_SOUND_FILE_NAME, "wb");
#ifdef VERSION_2
	int mod = (texbuf->size() % 8); 
	if(mod != 0)
	{
		//Texsize += (8-mod) * 2;
		for(int k = 0 ;k != 8 - mod ; mod ++)
		{
			texbuf->push_back(0);
		}
	}
	int TexBaseAddr=VectorDataSize + texbuf->size()*sizeof(u16);
#endif
	*Start = TexBaseAddr;
	*size = 0;
	//duration = 1000/freq
	int channels = 2;
    int frameCount = ceil(audioSampleRate * 1000/frequence/1000);
	for (int i = 0; i < frameCount; i++) {
        double t =  1.0*i / frameCount * 1000/frequence/1000;
        s16 curValue = sin(2.0*M_PI*frequence * t) * 32767;
		texbuf->push_back(curValue);
		fwrite(&curValue, 1, 2, fp);
		texbuf->push_back(curValue);
		fwrite(&curValue, 1, 2, fp);
		*size += 4;
    }
	fclose(fp);
	return 1;
}

int SoundVector::generateFromSoundFile(std::string s,u32 audioSampleRate,u32 *Start,u32 *size){
	FILE * fp = fopen(AHMI_SOUND_FILE_NAME, "wb");
#ifdef VERSION_2
	int mod = (texbuf->size() % 8); 
	if(mod != 0)
	{
		//Texsize += (8-mod) * 2;
		for(int k = 0 ;k != 8 - mod ; mod ++)
		{
			texbuf->push_back(0);
		}
	}
	int TexBaseAddr=VectorDataSize + texbuf->size()*sizeof(u16);
#endif
	*Start = TexBaseAddr;
	*size = 0;
	AVFormatContext	*pFormatCtx;
	int				i, audioStream;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVPacket		*packet;
	uint8_t			*out_buffer;
	AVFrame			*pFrame;
	
    int ret;
	uint32_t len = 0;
	uint32_t startTime = 0;
	int got_picture;
	int index = 0;
	int64_t in_channel_layout;
	struct SwrContext *au_convert_ctx;
 
	//FILE *pFile=NULL;
	char *url=(char *)s.c_str();
 
	//av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	//Open
	if(avformat_open_input(&pFormatCtx,url,NULL,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
	// Retrieve stream information
	if(avformat_find_stream_info(pFormatCtx,NULL)<0){
		printf("Couldn't find stream information.\n");
		return -1;
	}
	// Dump valid information onto standard error
	av_dump_format(pFormatCtx, 0, url, false);
 
	// Find the first audio stream
	audioStream=-1;
	for(i=0; i < pFormatCtx->nb_streams; i++)
		if(pFormatCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_AUDIO){
			audioStream=i;
			startTime = pFormatCtx->streams[i]->start_time;
			break;
		}
 
	if(audioStream==-1){
		printf("Didn't find a audio stream.\n");
		return -1;
	}
 
	// Get a pointer to the codec context for the audio stream
	//pCodecCtx=pFormatCtx->streams[audioStream]->codec;
	pCodecCtx = avcodec_alloc_context3(NULL);
	if (pCodecCtx == NULL)
	{
		printf("Could not allocate AVCodecContext.\n");
	}
	avcodec_parameters_to_context(pCodecCtx,pFormatCtx->streams[audioStream]->codecpar);
	// Find the decoder for the audio stream
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL){
		printf("Codec not found.\n");
		return -1;
	}
 
	// Open codec
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
		printf("Could not open codec.\n");
		return -1;
	}

	packet=(AVPacket *)av_malloc(sizeof(AVPacket));
	//printf("%d",sizeof(AVPacket));
	av_init_packet(packet);
 
	//Out Audio Param
	uint64_t out_channel_layout=AV_CH_LAYOUT_STEREO;
	//nb_samples: AAC-1024 MP3-1152
	int out_sample_rate=audioSampleRate;
	
	AVSampleFormat out_sample_fmt=AV_SAMPLE_FMT_S16;
	
	int out_channels=av_get_channel_layout_nb_channels(out_channel_layout);
	//Out Buffer Size
	//int out_buffer_size=av_samples_get_buffer_size(NULL,out_channels ,out_nb_samples,out_sample_fmt, 1);
 
	out_buffer=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);
	pFrame=av_frame_alloc();

		//FIX:Some Codec's Context Information is missing
	in_channel_layout=av_get_default_channel_layout(pCodecCtx->channels);
	//Swr
 
	au_convert_ctx = swr_alloc();
	au_convert_ctx=swr_alloc_set_opts(au_convert_ctx,out_channel_layout, out_sample_fmt, out_sample_rate,
		in_channel_layout,pCodecCtx->sample_fmt , pCodecCtx->sample_rate,0, NULL);
	swr_init(au_convert_ctx);

	int out_nb_samples;
	//last pkt 
	bool lastPktOdd = false;
	u8 lastPktRemained = 0;//if lastPkt is odd, this is useful
	while(av_read_frame(pFormatCtx, packet)>=0){
		if(packet->stream_index==audioStream){
			//ret = avcodec_decode_audio4( pCodecCtx, pFrame,&got_picture, packet);
			ret = avcodec_send_packet(pCodecCtx, packet);
			if ( ret != 0 ) {
				printf("Error in decoding audio frame.\n");
				return -1;
			}
			got_picture = avcodec_receive_frame(pCodecCtx, pFrame); //got_picture = 0 success, a frame was returned

			if ( got_picture == 0 ){
				//int out_nb_samples=pCodecCtx->frame_size*out_sample_rate/pCodecCtx->sample_rate;
				out_nb_samples= ceil(pFrame->nb_samples*out_sample_rate/pCodecCtx->sample_rate);
				//int out_nb_samples= pFrame->nb_samples;
				//int fifo_size = swr_get_out_samples(au_convert_ctx,pFrame->nb_samples);
				ret = swr_convert(au_convert_ctx,&out_buffer, out_nb_samples,(const uint8_t **)pFrame->data , pFrame->nb_samples);
				//ret = swr_convert(au_convert_ctx,&out_buffer, fifo_size,0 , 0);
				//fifo_size = swr_get_out_samples(au_convert_ctx,0);
				//out_buffer = pFrame->data[0];
	#if 1
				//printf("index:%5d\t pts:%lld\t packet size:%d samples:%d\n",index,packet->pts,packet->size,out_nb_samples);
	#endif
				int out_buffer_size=av_samples_get_buffer_size(NULL,out_channels ,ret,out_sample_fmt, 1);
				if(ret > 0)
				{
					//add to texbuf
					int cur_out_buffer_size = out_buffer_size;
					int start_out_buffer = 0;

					if (lastPktOdd)
					{
						//combine lastPktRemained and first u8 in this pkt
						u16 t = (out_buffer[start_out_buffer]<<8)+lastPktRemained;
						start_out_buffer++;
						texbuf->push_back(t);
						//write to file
						fwrite(&t, 1, 2, fp);
						cur_out_buffer_size--;
					}
					if (cur_out_buffer_size%2)
					{
						//odd
						lastPktOdd = true;
						lastPktRemained = out_buffer[out_buffer_size-1];
					}else{
						//even
						lastPktOdd = false;
					}
					for (int i = start_out_buffer; i < out_buffer_size; i = i+2)
					{
						u16 t = (out_buffer[i+1]<<8) + out_buffer[i];
						texbuf->push_back(t);
						//write to file
						fwrite(&t, 1, 2, fp);
					}

	
					*size += out_buffer_size;
				}
 
				
				index++;
			}
		}
		//av_free_packet(packet);
		av_packet_unref(packet);
	}
	//process last buffer in swr
	out_nb_samples = swr_get_out_samples(au_convert_ctx,0);
	ret = swr_convert(au_convert_ctx,&out_buffer,out_nb_samples ,NULL,0);
	int out_buffer_size=av_samples_get_buffer_size(NULL,out_channels ,ret,out_sample_fmt, 1);
	if(ret > 0){
		//add to texbuf
		int cur_out_buffer_size = out_buffer_size;
		int start_out_buffer = 0;

		if (lastPktOdd)
		{
			//combine lastPktRemained and first u8 in this pkt
			u16 t = (out_buffer[start_out_buffer]<<8)+lastPktRemained;
			start_out_buffer++;
			texbuf->push_back(t);
			//write to file
			fwrite(&t, 1, 2, fp);
			cur_out_buffer_size--;
		}
		if (cur_out_buffer_size%2)
		{
			//odd
			lastPktOdd = true;
			lastPktRemained = out_buffer[out_buffer_size-1];
		}else{
			//even
			lastPktOdd = false;
		}
		for (int i = start_out_buffer; i < out_buffer_size; i = i+2)
		{
			u16 t = (out_buffer[i+1]<<8) + out_buffer[i];
			texbuf->push_back(t);
			//write to file
			fwrite(&t, 1, 2, fp);
		}
		*size += out_buffer_size;
	}
	//handle last odd pkt
	if (lastPktOdd)
	{
		u16 t = (0<<8)+lastPktRemained;//append a 0
		texbuf->push_back(t);
		//write to file
		fwrite(&t, 1, 2, fp);
		*size++;
	}
	//// 4 byte align
	//int sizeToFill = 4 - (*size % 4);
	//if (sizeToFill>0)
	//{
	//	char* padding = (char*)malloc(sizeof(char)*(sizeToFill));
	//	for (int i = 0; i < sizeToFill; i++)
	//	{
	//		padding[i] = 0;
	//	}
	//	fwrite(padding,1,sizeToFill,fp);
	//	*size += sizeToFill;
	//	free(padding);
	//}
	
	//printf("sound over\n");
	swr_free(&au_convert_ctx);
	av_free(out_buffer);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
	fclose(fp);
	return 1;
}

int SoundVector::flush(ConfigBuf * pConfigBuf){
	SoundClass temsound;
	for(SoundVector::iterator it = begin();it!=end();it++){
		temsound.mSoundID = it->mSoundID;
		temsound.genSineFreq = it->genSineFreq;
		temsound.loop = it->loop;
		temsound.mStartAddr = it->mStartAddr;
		temsound.mSize = it->mSize;
		temsound.sineFreq = it->sineFreq;
		temsound.sineDuration = it->sineDuration;
		pConfigBuf->Write(&temsound);
	
	
	}
	return 0;
}

int SoundVector::flushAndRecordTextureAddr(ConfigBuf * pConfigBuf, vector<struct DelayCalcValue>* delayCalcValueList){
	SoundClass temsound;
	for(SoundVector::iterator it = begin();it!=end();it++){
		temsound.mSoundID = it->mSoundID;
		temsound.genSineFreq = it->genSineFreq;
		temsound.loop = it->loop;
		temsound.mStartAddr = it->mStartAddr;
		temsound.mSize = it->mSize;
		temsound.sineFreq = it->sineFreq;
		temsound.sineDuration = it->sineDuration;

		////add to delay calc value list
		//DelayCalcValue dcv;
		//u32 offset = (u32)&(temsound.mStartAddr) - (u32)&(temsound);
		//dcv.addr = pConfigBuf->size() + offset;
		//dcv.ogValue = temsound.mStartAddr;
		//dcv.TypeSize = sizeof(u32);

		//delayCalcValueList->push_back(dcv);

		pConfigBuf->Write(&temsound);
	
	
	}
	return 0;
}

#endif


