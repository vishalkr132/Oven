#base image
FROM gcc:latest

#set the working directory
WORKDIR /app

#copy the project into docker container
COPY ..

#install any dependency pt install -y cmake  --> exampleckages
#RUN apt-update && apt-get install -y cmake  --> example

#compile the cpp and c files
RUN g++ -o main service/main.cpp service/service.cpp hwInterface/uart/odor.c hwInterface/uart/radar.c hwInterface/uart/temp.c hwInterface/tcp_ip/tcp.c

#command to run the application

CMD[./main]