cmake_minimum_required(VERSION 3.25)
project(Project_V2)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp
        src/CSV.cpp include/CSV.h
        src/Miscellaneous.cpp include/Miscellaneous.h
        src/Date.cpp include/Date.h
        src/Cities.cpp include/Cities.h
        src/Student.cpp include/Student.h
        src/StudentMap.cpp include/StudentMap.h
        src/StudentHelper.cpp include/StudentHelper.h
        src/StudentList.cpp include/StudentList.h
        src/Course.cpp include/Course.h
        src/CourseMap.cpp include/CourseMap.h
        src/CourseList.cpp include/CourseList.h
        src/CourseHelper.cpp include/CourseHelper.h
        src/StudentErrorFlag.cpp include/StudentErrorFlag.h
        src/CourseErrorFlag.cpp include/CourseErrorFlag.h)

# Copy folder GenderIdentification to binary folder
file(COPY GenderIdentification/ DESTINATION GenderIdentification/)
# Copy CityData to binary folder
file(COPY CityData.txt DESTINATION /)