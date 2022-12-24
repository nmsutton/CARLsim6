int EC_LI_II_Multipolar_Pyramidal=sim.createGroup("EC_LI_II_Multipolar_Pyramidal", 
                            1000, EXCITATORY_NEURON, ANY, GPU_CORES); 
int MEC_LII_Stellate=sim.createGroup("MEC_LII_Stellate", 
                            1000, EXCITATORY_NEURON, ANY, GPU_CORES);

sim.setNeuronParameters(EC_LI_II_Multipolar_Pyramidal, 204.0f, 0.0f, 0.37f, 0.0f, -70.53f, 0.0f, -39.99f, 
                                0.0f, 0.001f, 0.0f, 0.01f, 0.0f, 3.96f, 0.0f, -54.95f, 0.0f, 
                                7.0f, 0.0f, 0);
sim.setNeuronParameters(MEC_LII_Stellate, 118.0f, 0.0f, 0.98f, 0.0f, -58.53f, 0.0f, -43.52f, 
                                0.0f, 0.004f, 0.0f, 7.0f, 0.0f, 7.85f, 0.0f, -52.68f, 0.0f, 
                                65.0f, 0.0f, 0);

/* neuron connection parameters */
sim.connect(EC_LI_II_Multipolar_Pyramidal, MEC_LII_Stellate, "full", 10.0f, 1.0f, 
    RangeDelay(1), RadiusRF(-1), SYN_FIXED, 1, 1);

/* STP parameters */
sim.setSTP(EC_LI_II_Multipolar_Pyramidal, MEC_LII_Stellate, true, STPu(0.1513, 0.0f),
                                     STPtauU(69.11, 0.0f),
                                     STPtauX(98.1, 0.0f),
                                     STPtdAMPA(6.0, 0.0f),
                                     STPtdNMDA(150.0, 0.0f),
                                     STPtdGABAa(5.0, 0.0f),
                                     STPtdGABAb(150.0, 0.0f),
                                     STPtrNMDA(0.0f, 0.0f),
                                     STPtrGABAb(0.0f, 0.0f));