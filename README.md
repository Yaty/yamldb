# YAML DB

This repository is a team project built by :
- Alexis Benard
- Vincent Guenin
- Hugo Da Roit

We have done this project for our ESGI C courses.

We have implemented a basic UI to be able to create tables, columns, etc.
You can also execute queries on data (select, update, delete, insert, order by, where, joins ...) (https://github.com/Yaty/yamldb/blob/master/documentation/sql.md)[more info here.]

In this project sources you will be able to find a basic YAML parser which can handle maps and sequences. There is also a serializer.
All the code is in ./src/code. You can find what's about SQL in the sql directory and the YAML parser in the yaml directory. For the parser there is a lot of useful functions which are all documented (https://github.com/Yaty/yamldb/blob/master/documentation/parser.md)[here.]

We also have done unit testing, you can find them in ./tests.

To build this project use CMake : `cmake CMakeLists.txt`
You can also use it in your favorite IDE, we used CLion for this project.

It was a pleasure to code this project, if you have any questions you are free to ask :)
