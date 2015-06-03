cmake_minimum_required(VERSION 2.8)
project(SATenstein)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -O3 -lm ")

set(SOURCE_FILES
                    src/adaptnovelty.c
                    src/algorithms.c
                    src/derandomized.c
                    src/ddfw.c
                    src/satenstein.c
                    src/gsat.c
                    src/gsat-tabu.c
                    src/gwsat.c
                    src/hsat.c
                    src/hwsat.c
                    src/irots.c
                    src/mt19937ar.c
                    src/mylocal.c
                    src/novelty.c
                    src/parameters.c
                    src/paws.c
                    src/random.c
                    src/reports.c
                    src/rgsat.c
                    src/rnovelty.c
                    src/rots.c
                    src/samd.c
                    src/saps.c
                    src/ubcsat.c
                    src/ubcsat-help.c
                    src/ubcsat-internal.c
                    src/ubcsat-io.c
                    src/ubcsat-mem.c
                    src/ubcsat-reports.c
                    src/ubcsat-time.c
                    src/ubcsat-triggers.c
                    src/vw.c
                    src/walksat.c
                    src/walksat-tabu.c
                    src/dcca.c
)

add_executable(ubcsat ${SOURCE_FILES} src/satenstein-types.h)