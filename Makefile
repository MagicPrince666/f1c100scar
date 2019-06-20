CXX = arm-f1c100s-linux-uclibcgnueabi-g++
TARGET1	= prince

DIR		= .
INC		= -I. 
CFLAGS	= -Wall
LDFLAGS = 

OBJPATH	= .

FILES	= $(foreach dir,$(DIR),$(wildcard $(dir)/*.cpp))

OBJS	= $(patsubst %.cpp,%.o,$(FILES))

all:$(OBJS) $(TARGET1)

$(OBJS):%.o:%.cpp
	$(CXX) $(CFLAGS) $(INC) -c -o $(OBJPATH)/$(notdir $@) $< 

$(TARGET1):$(OBJPATH)
	$(CXX) -o $@ $(OBJPATH)/*.o $(LDFLAGS)

install:
	../file/xag_tcpscs 169.254.140.55 5555 $(TARGET1)

clean:
	-rm $(OBJPATH)/*.o
	-rm $(TARGET1)
	
