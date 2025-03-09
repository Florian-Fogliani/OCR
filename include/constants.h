#define L1 784
#define L2 30
#define L3 10

#define B L2 + L3
#define W2 L1 *L2
#define W3 L2 *L3
#define W W2 + W3
#define P (W + B)

#define EPOCHS 80000
#define TRAININGS 60000
#define BATCH_SIZE 100
#define RATE 0.003
#define DECAY 0.999
