TARGET = CombinedFitPlus

OBJS_CXX = $(TARGET).o FitMaster.o SampleROOT.o WeightCalc.o OscWeightCalc.o ExpWeightCalc.o ResonanceWeightCalc.o ResExtraDimWeightCalc.o POTWeightCalc.o Consts.o ErrorMatrixROOT.o \
			Util.o Algo.o AlgoSensitivity.o AlgoFit.o Filter.o MsgLog.o FuncStopwatch.o
SRCS = $(TARGET).C FitMaster.cxx Sample.cxx Filter.cxx ErrorMatrixROOT.cxx Util.cxx Consts.cxx MsgLog.cxx FuncStopwatch.cxx

ROOTLIBS = $(shell root-config --glibs)  $(shell root-config --libs)  -lXMLParser -lMinuit -lXMLIO -lXMLParser
EXTRALIBS =  -lm

CPPFLAGS   +=  -O3 -std=c++11
CPPFLAGS   +=   $(shell root-config --cflags)

#CC = @echo Compiling $@; g++
LD = @echo Linking $@; ld
#LD = ld
CC = g++ -Wall -fno-exceptions -fPIC -g

%.o: %.C
	@echo Compiling $*.o
	$(CC) -c $*.C  $(CPPFLAGS)

%.o: %.cxx
	$(CC) -c $*.cxx  $(CPPFLAGS) 

$(TARGET) : $(OBJS_CXX) $(OBJS_F77)
	@echo target; $(CC) $(CPPFLAGS) $(OBJS_CXX) $(OBJS_F77) -o $(TARGET)  $(ROOTLIBS) $(EXTRALIBS)
	@cd ../bin; ln -s -f ../src/$(TARGET)

allWC.o : WeightCalc.cxx WeightCalc.h OscWeightCalc.cxx OscWeightCalc.h ExpWeightCalc.cxx ExpWeightCalc.h ResonanceWeightCalc.cxx ResonanceWeightCalc.h ResExtraDimWeightCalc.cxx ResExtraDimWeightCalc.h POTWeightCalc.cxx POTWeightCalc.h Consts.h MsgLog.h FuncStopwatch.h
	$(CC) -c WeightCalc.cxx OscWeightCalc.cxx ExpWeightCalc.cxx ResonanceWeightCalc.cxx ResExtraDimWeightCalc.cxx POTWeightCalc.cxx $(CPPFLAGS) $(ROOTLIBS) 
#	$(LD) -o $@ *WeightCalc.o
#	$(LD) -o $@ -Ur *WeightCalc.o

Algorithm.o : Algo.cxx Algo.h AlgoSensitivity.cxx AlgoSensitivity.h AlgoFit.cxx AlgoFit.h FitMaster.h Util.h MsgLog.h FuncStopwatch.h
#	$(CC) -c Algo.cxx AlgoSensitivity.cxx AlgoFit.cxx  $(CPPFLAGS) $(ROOTLIBS)
	$(CC) -c Algo.cxx AlgoSensitivity.cxx AlgoFit.cxx  $(ROOTLIBS)
#	$(LD) -o $@ Algo*.o
#	$(LD) -o $@ -Ur Algo*.o

Consts.o: Consts.cxx Consts.h

ErrorMatrixROOT.o: ErrorMatrixROOT.cxx ErrorMatrixROOT.h Consts.h Event.h

Filter.o: Filter.cxx Filter.h Event.h

FitMaster.o: FitMaster.cxx FitMaster.h ErrorMatrixROOT.h Consts.h Util.h SampleROOT.h WeightCalc.h Filter.h \
             Algo.h Event.h AlgoSensitivity.h AlgoFit.h POTWeightCalc.h OscWeightCalc.h ExpWeightCalc.h ResonanceWeightCalc.h ResExtraDimWeightCalc.h MsgLog.h FuncStopwatch.h

SampleROOT.o: SampleROOT.cxx Event.h Consts.h SampleROOT.h WeightCalc.h Filter.h MsgLog.h FuncStopwatch.h

Util.o: Util.cxx Util.h ErrorMatrixROOT.h Consts.h FitMaster.h SampleROOT.h WeightCalc.h Filter.h Algo.h MsgLog.h FuncStopwatch.h

CombinedFitPlus.o: CombinedFitPlus.C FitMaster.h ErrorMatrixROOT.h Consts.h Util.h SampleROOT.h WeightCalc.h \
                   Filter.h Algo.h AlgoSensitivity.h OscWeightCalc.h ExpWeightCalc.h ResonanceWeightCalc.h ResExtraDimWeightCalc.h POTWeightCalc.h MsgLog.h FuncStopwatch.h





.PHONY : clean

libWeight :  WeightCalc.cxx WeightCalc.h OscWeightCalc.cxx OscWeightCalc.h ExpWeightCalc.cxx ResonanceWeightCalc.cxx ResExtraDimWeightCalc.cxx ExpWeightCalc.h ResonanceWeightCalc.h ResExtraDimWeightCalc.h POTWeightCalc.cxx POTWeightCalc.h
	rootcint WeightCalcDict.C -c WeightCalc.h OscWeightCalc.h ExpWeightCalc.h ResonanceWeightCalc.h ResExtraDimWeightCalc.h POTWeightCalc.h
	g++ -Wall -fPIC -c WeightCalc.cxx OscWeightCalc.cxx ExpWeightCalc.cxx ResonanceWeightCalc.cxx ResExtraDimWeightCalc.cxx POTWeightCalc.cxx WeightCalcDict.C $(CPPFLAGS) $(ROOTLIBS)
	g++ -shared -Wl,-soname,libWeight.so -o libWeight.so *WeightCalc*.o
#	ld -G *WeightCalc.o -o libWeight.so

clean :
	@echo Cleaning...
	-@rm -f ../src/*.o 
	-@rm -f ../src/$(TARGET) 
	-@rm -f ../bin/$(TARGET)

