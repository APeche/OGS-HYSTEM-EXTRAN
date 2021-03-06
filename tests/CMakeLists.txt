IF( MSVC ) # VS2012 doesn't support correctly the tuples yet
	ADD_DEFINITIONS( /D _VARIADIC_MAX=10 )
ENDIF()

ADD_SUBDIRECTORY( gtest )

# Add tests which do not need testdata here:
SET ( SOURCES
	testrunner.cpp
	testBase.cpp
)

# Add tests here if they need testdata
IF(TESTDATA_DIR_FOUND)
	SET(SOURCES ${SOURCES}
		GEO/TestPointInTriangle.cpp
		GEO/TestPointInPolygon.cpp
	)
ENDIF()

IF (NOT MSVC)
	SET ( SOURCES ${SOURCES}
		testOgs.cxx
		testBmSkel.cxx)
ENDIF (NOT MSVC)

SET ( SOURCES ${SOURCES}
	Matrix/testMatrix.cpp
    )

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/tests/gtest
	${CMAKE_SOURCE_DIR}/Base
	${CMAKE_SOURCE_DIR}/FileIO
	${CMAKE_SOURCE_DIR}/GEO
	${CMAKE_SOURCE_DIR}/MathLib
	${CMAKE_SOURCE_DIR}/FEM
	${CMAKE_SOURCE_DIR}/MSH
	${CMAKE_SOURCE_DIR}/OGSProject
)

ADD_EXECUTABLE (testrunner ${SOURCES})
TARGET_LINK_LIBRARIES(testrunner
  GTest
  Base
  FileIO
  GEO
  MathLib
  FEM
  MSH
  OGSProject
)

IF (MSVC)
	SET(testrunnerExe ${EXECUTABLE_OUTPUT_PATH}/Release/testrunner)
	SET(QtTestrunnerExe ${EXECUTABLE_OUTPUT_PATH}/Release/QtTestrunner)
ELSE ()
	SET(testrunnerExe ${EXECUTABLE_OUTPUT_PATH}/testrunner)
	IF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
		TARGET_LINK_LIBRARIES( testrunner pthread )
	ENDIF() # "${CMAKE_SYSTEM_NAME}" MATCHES "Linux"
	SET (QtTestrunnerExe ${EXECUTABLE_OUTPUT_PATH}/QtTestrunner)
ENDIF() # MSVC

# Add targets for running tests (with coverage reports)
IF(OGS_COVERAGE)
	# Use cobertura output on Jenkins
	IF(JENKINS_URL)
		SETUP_TARGET_FOR_COVERAGE_COBERTURA(gtests_cobertura testrunner "gtests_coverage_results")
		IF(BENCHMARK_DIR_FOUND)
			SETUP_TARGET_FOR_COVERAGE_COBERTURA(benchmark_coverage ctest "benchmark_coverage_results" "-E;'Tests|FILE|EXCEED';-j;${PROCESSOR_COUNT}")
		ENDIF() # BENCHMARK_DIR_FOUND
	# Use lcov output else
	ELSE()
		SETUP_TARGET_FOR_COVERAGE(gtests testrunner "gtests_coverage_results")
		IF(BENCHMARK_DIR_FOUND)
			SETUP_TARGET_FOR_COVERAGE(benchmark_coverage ctest "benchmark_coverage_results" "-E;'Tests|FILE|EXCEED';-j;${PROCESSOR_COUNT}")
		ENDIF() # BENCHMARK_DIR_FOUND
	ENDIF() # JENKINS_URL

ELSE() # without coverage reports
	ADD_CUSTOM_TARGET(gtests testrunner	DEPENDS testrunner)
	SET_TARGET_PROPERTIES(testrunner gtests PROPERTIES FOLDER Testing)
ENDIF() # OGS_COVERAGE

# Either collect all gtests in one ctest run of testrunner executable
# or run each gtest individually
IF (OGS_ONE_BIG_GTEST)
	add_test (GTests ${testrunnerExe}  --gtest_output=xml:./)
ELSE (OGS_ONE_BIG_GTEST)
	ADD_GOOGLE_TESTS ( ${testrunnerExe} ${SOURCES})
ENDIF (OGS_ONE_BIG_GTEST)

# add new test models here not all of these models will be needed, depending
# on compile flags but it is safe to copy them all across
IF (NOT MSVC)
	ADD_SUBDIRECTORY( data/bmskel )
ENDIF (NOT MSVC)
