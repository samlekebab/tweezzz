#ifndef CORE
#define CORE
#include "aom1D.h"
#include "aom2D.h"
#include "scheduler.h"
#include <chrono>
#include "coregpu.h"
#include "setting.h"

#ifndef no_card_connected
#include "card.h"
#endif

//TODO update this comment  
//this is where the magic is : 
//first, core is asking the scheduler if there is the need to rewind. thene they agree to wich segment to calculate (bunch of samples)
//then, the scheduler will call the function generators to calculate the current tick
//third, the core calculate the complete function for each channel x, as sum_i(Axi * A_x * N_xi * N_x * sin( w_xi t + p_i)
//finaly, the core send it to the card (hopfully in parallel ?) to the right buffer TODO
//(curently in fifo, try sequence replay mode) :
//a buffer is an assembly of segments. they are needed to preload forms in the card durring mesurement and thene reducing the time between the outcome of a mesurement and feddback to the system. (expected to be about 250us with this method instead of about 750us in a naive FIFO implementation) (to test) TODO(currently fifo)
//yield to let eventual form Generators to register on the scheduler (will this yield reduce the bandwidth and prevent relieable output ?
//this function need to be started on a new thread

namespace coreCalc{

void startTimer();
long getTimer(std::chrono::time_point<std::chrono::system_clock> timer);

void startCore(Scheduler& scheduler, void (*sequence)(Aom1D&,Aom2D&), Aom1D& aom1D, Aom2D& aom2D);

void calculateCPU(Scheduler& scheduler, Aom1D& aom1D, Aom2D& aom2D, long currentTick, int16_t* buff);
void calculateSegment(Scheduler& scheduler, Aom1D& aom1D, Aom2D& aom2D, long tick, int16_t* segment_buffer);
void calculate_tick(const Aom1D aom1D,const Aom2D aom2D,const long tick, int16_t& buff);
void calculate_tweezer(Aom1D& aom1D,Aom2D& aom2D,int tweezer, long initial_tick, int16_t* buff);

int16_t* calculateGPU(Scheduler& scheduler, Aom& aom, long currentTick, CoreGPU& gpu, int16_t* buffer);


long findTickAssociatedToSegment(int segement, long lastTick);
#ifdef no_card_connected
int getCurrentCardSegment();
#else
int getCurrentCardSegment(Card& card);
#endif
void pushSegmentToCard(int segment,int16_t data);
inline int findSegmentAssociatedToTick(long tick);

void copyTo4chBuff(int16_t* dest,int16_t* src,size_t length,float factor);
}

#endif
