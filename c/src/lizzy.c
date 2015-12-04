#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/select.h>

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>

#include "pocketsphinx.h"

static const arg_t cont_args_def[] = {
    POCKETSPHINX_OPTIONS,
    /* Argument file. */
    {"-argfile",
     ARG_STRING,
     NULL,
     "Argument file giving extra arguments."},
    {"-adcdev",
     ARG_STRING,
     NULL,
     "Name of audio device to use for input."},
    {"-infile",
     ARG_STRING,
     NULL,
     "Audio file to transcribe."},
    {"-inmic",
     ARG_BOOLEAN,
     "no",
     "Transcribe audio from microphone."},
    {"-time",
     ARG_BOOLEAN,
     "no",
     "Print word times in file transcription."},
    CMDLN_EMPTY_OPTION
};


static void
sleep_msec(int32 ms)
{
    struct timeval tmo;

    tmo.tv_sec = 0;
    tmo.tv_usec = ms * 1000;

    select(0, NULL, NULL, NULL, &tmo);
}

int main(int argc, char **argv) {
    char const *cfg;
    ps_decoder_t *ps;
    cmd_ln_t *config;
    
    config = cmd_ln_parse_r(NULL, cont_args_def, argc, argv, TRUE);

    /* Handle argument file as -argfile. */
    if (config && (cfg = cmd_ln_str_r(config, "-argfile")) != NULL) {
        config = cmd_ln_parse_file_r(config, cont_args_def, cfg, FALSE);
    }

    if (config == NULL || (cmd_ln_str_r(config, "-infile") == NULL && cmd_ln_boolean_r(config, "-inmic") == FALSE)) {
	E_INFO("Specify '-infile <file.wav>' to recognize from file or '-inmic yes' to recognize from microphone.\n");
        cmd_ln_free_r(config);
	return 1;
    }

    ps_default_search_args(config);
    ps = ps_init(config);
    if (ps == NULL) {
        cmd_ln_free_r(config);
        return 1;
    }

    E_INFO("%s COMPILED ON: %s, AT: %s\n\n", argv[0], __DATE__, __TIME__);

    ad_rec_t *ad;
//    int16 adbuf[4096];
    uint8 utt_started, in_speech;
    int32 k;
    char const *hyp;

    if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"), (int) cmd_ln_float32_r(config, "-samprate"))) == NULL) {
        E_FATAL("Failed to open audio device\n");
    }
  
    if (ad_start_rec(ad) < 0) {
        E_FATAL("Failed to start recording\n");
    }

    if (ps_start_utt(ps) < 0) {
        E_FATAL("Failed to start utterance\n");
    }

    utt_started = FALSE;
    printf("READY before loop....\n");

    for (;;) {

	int16 adbuf[4096];

        if ((k = ad_read(ad, adbuf, 4096)) < 0) {
            E_FATAL("Failed to read audio\n");
	}

        ps_process_raw(ps, adbuf, k, FALSE, FALSE);

        in_speech = ps_get_in_speech(ps);

        if (in_speech && !utt_started) {
            utt_started = TRUE;
            printf("Listening...\n");
        }

        if (!in_speech && utt_started) {
            /* speech -> silence transition, time to start new utterance  */
            ps_end_utt(ps);

            hyp = ps_get_hyp(ps, NULL );

            if (hyp != NULL) {
                printf("hyp == %s\n", hyp);
	    }

            if (ps_start_utt(ps) < 0) {
                E_FATAL("Failed to start utterance\n");
	    }

            utt_started = FALSE;
            printf("READY in loop....\n");
        }
        sleep_msec(10);
    }

    ad_close(ad);
    ps_free(ps);
    cmd_ln_free_r(config);

    return 0;
}
