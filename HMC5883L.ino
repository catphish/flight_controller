int16_t mx, my, mz;

void hmlGetHeading() {
  mag.getHeading(&mx, &my, &mz);
  
  heading = atan2(my, mx);
  if(heading < 0)
    heading += 2 * M_PI;
}
