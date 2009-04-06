
typedef struct wav_hdr {
  unsigned int   magic;
  unsigned int   length;
  unsigned int   chunk_type;
  unsigned int   chunk_format;
  unsigned int   chunk_length;
  unsigned short format;
  unsigned short channels;
  unsigned int   sample_rate;
  unsigned int   bytes_per_second;
  unsigned short sample_size;
  unsigned short precision;
  unsigned int   chunk_data;
  unsigned int   data_length;
};

/* little and big-endian handling - RIFF/WAV files are always stored in BigEndian */
union swap  {
  unsigned long  ul;
  unsigned char  uc[4];
  unsigned short us;
};

static __inline unsigned short sswap (unsigned short us)  {
  union swap ul1, ul2;
  
  ul1.us    = us;
  ul2.uc[0] = ul1.uc[1];
  ul2.uc[1] = ul1.uc[0];
  return (ul2.us);
}

static __inline unsigned long  lswap (unsigned long ul)  {
  union swap ul1, ul2;
  
  ul1.ul    = ul;
  ul2.uc[0] = ul1.uc[3];
  ul2.uc[1] = ul1.uc[2];
  ul2.uc[2] = ul1.uc[1];
  ul2.uc[3] = ul1.uc[0];
  return (ul2.ul);
}

void tc0 ();
void init_timer1_for_audio (void);


