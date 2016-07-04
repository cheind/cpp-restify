cmake_minimum_required(VERSION 3.0)

project(mongoose)

# Library

add_library(mongoose
	mongoose.h
	mongoose.c
)

 install(FILES mongoose.h DESTINATION inc/mongoose)
 install(TARGETS mongoose EXPORT mongoose-targets DESTINATION lib/mongoose)
 install(EXPORT mongoose-targets DESTINATION lib/mongoose)

