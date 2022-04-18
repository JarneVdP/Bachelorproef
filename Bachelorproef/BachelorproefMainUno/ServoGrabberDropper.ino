void servoGrab(){
    samplestored[numberofsamplestored] = id_ard;
}

void servoDrop(){
    samplestored[numberofsamplestored] = '\0';
    numberofsamplestored--;
}