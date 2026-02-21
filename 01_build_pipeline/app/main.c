int main(void){
    volatile int counter = 0;
    while(1){
        counter++;
        if(counter > 1000) counter = 0;
    }
    return 0;
}
