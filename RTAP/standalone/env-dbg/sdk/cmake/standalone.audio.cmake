header_message("Configuring audio submodule")

# include platform specific stuff
include(${JVX_SUBPRODUCT_ROOT}/cmake/${JVX_OS}_${JVX_PRODUCT}.cmake)

###
# configure packages specific to rtproc subproduct
###

# configure libsndfile
find_sndfile()

# configure platform specific stuff (e.g. alsa)
find_platform_specific()

