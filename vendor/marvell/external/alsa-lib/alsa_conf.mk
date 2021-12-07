lib_path := vendor/marvell/external/alsa-lib
alsa_path := system/usr/share/alsa

PRODUCT_COPY_FILES += \
	$(lib_path)/src/conf/alsa.conf:$(alsa_path)/alsa.conf \
	$(lib_path)/src/conf/pcm/dsnoop.conf:$(alsa_path)/pcm/dsnoop.conf \
	$(lib_path)/src/conf/pcm/modem.conf:$(alsa_path)/pcm/modem.conf \
	$(lib_path)/src/conf/pcm/dpl.conf:$(alsa_path)/pcm/dpl.conf \
	$(lib_path)/src/conf/pcm/default.conf:$(alsa_path)/pcm/default.conf \
	$(lib_path)/src/conf/pcm/surround51.conf:$(alsa_path)/pcm/surround51.conf \
	$(lib_path)/src/conf/pcm/surround41.conf:$(alsa_path)/pcm/surround41.conf \
	$(lib_path)/src/conf/pcm/surround50.conf:$(alsa_path)/pcm/surround50.conf \
	$(lib_path)/src/conf/pcm/dmix.conf:$(alsa_path)/pcm/dmix.conf \
	$(lib_path)/src/conf/pcm/center_lfe.conf:$(alsa_path)/pcm/centor_lfe.conf \
	$(lib_path)/src/conf/pcm/surround40.conf:$(alsa_path)/pcm/surround40.conf \
	$(lib_path)/src/conf/pcm/side.conf:$(alsa_path)/pcm/side.conf \
	$(lib_path)/src/conf/pcm/iec958.conf:$(alsa_path)/pcm/iec958.conf \
	$(lib_path)/src/conf/pcm/rear.conf:$(alsa_path)/pcm/rear.conf \
	$(lib_path)/src/conf/pcm/surround71.conf:$(alsa_path)/pcm/surround71.conf \
	$(lib_path)/src/conf/pcm/front.conf:$(alsa_path)/pcm/front.conf \
	$(lib_path)/src/conf/cards/aliases.conf:$(alsa_path)/cards/aliases.conf
