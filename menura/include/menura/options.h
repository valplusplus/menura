namespace menura {
￼
￼   struct program_options {
￼     int sample_rate       = 44100;
￼     int frames_per_buffer = 1024;
￼     int max_bpm           = 120;
￼     // frequency spectrums calculated per second, using
￼     // Nyquist-doubled tempo.
￼     int spectrum_rate     = max_bpm / (60 * 2);
￼     // Number of samples used for frequency spectrum analysis.
￼     int num_samples       = (sample_rate / spectrum_rate);
￼
￼     int num_bins          = num_samples / 2;
￼
￼     // Number of audio input channels, defaulting to mono.
￼     int num_channels      = 1;
￼   };
￼
￼ }
