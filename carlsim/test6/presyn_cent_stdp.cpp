#include "gtest/gtest.h"
#include "carlsim_tests.h"
#include <carlsim.h>

TEST(presyn_cent_stdp, firingRate) {
	// ---------------- CONFIG STATE -------------------
	LoggerMode logger = SILENT;  // USER, SILENT
	int numGPUs = 1;
	int randSeed = 42;	
	CARLsim* sim = new CARLsim("test_presyn_cent", GPU_MODE, logger, numGPUs, randSeed);
	int EC_LI_II_Multipolar_Pyramidal=sim->createGroup("EC_LI_II_Multipolar_Pyramidal", 
	                            5, EXCITATORY_NEURON, ANY, GPU_CORES);  
	int MEC_LII_Stellate=sim->createGroup("MEC_LII_Stellate", 
	                            5, EXCITATORY_NEURON, ANY, GPU_CORES);	
	sim->setNeuronParameters(EC_LI_II_Multipolar_Pyramidal, 204.0f, 0.0f, 0.37f, 0.0f, -70.53f, 0.0f, -39.99f, 
	                                0.0f, 0.001f, 0.0f, 0.01f, 0.0f, 3.96f, 0.0f, -54.95f, 0.0f, 
	                                7.0f, 0.0f); // C,k,vr,vt,a,b,vpeak,c,d
	sim->setNeuronParameters(MEC_LII_Stellate, 118.0f, 0.0f, 0.98f, 0.0f, -58.53f, 0.0f, -43.52f, 
	                                0.0f, 0.004f, 0.0f, 7.0f, 0.0f, 7.85f, 0.0f, -52.68f, 0.0f, 
	                                65.0f, 0.0f);
	sim->connect(EC_LI_II_Multipolar_Pyramidal, MEC_LII_Stellate, "full", RangeWeight(0.0f, 50.0f, 100.0f), 1.0f, 
	    RangeDelay(1), RadiusRF(-1), SYN_PLASTIC, 1, 1);
	sim->setConductances(true, 7.55, 150.0, 6.0, 150.0);
	sim->setSTP(EC_LI_II_Multipolar_Pyramidal, true, 0.1513, 69.11, 98.1);
	sim->setSTP(MEC_LII_Stellate, true, 0.1513, 69.11, 98.1);
	sim->setESTDP(EC_LI_II_Multipolar_Pyramidal, MEC_LII_Stellate, true, STANDARD, ExpCurve(0.5f, 20.0f, -0.5f, 20.0f));
	//sim->setESTDP(EC_LI_II_Multipolar_Pyramidal, MEC_LII_Stellate, true, PKA_PLC_MOD, ExpCurve(0.1f, 20.0f, -0.1f, 20.0f), PkaPlcModulation(NM_NE, 1.0f, NM_ACh, 1.0f));
	sim->setWeightAndWeightChangeUpdate(INTERVAL_10MS, false, 0.5f);
	// ---------------- SETUP STATE -------------------
	sim->setupNetwork();
	SpikeMonitor* SpkMon = sim->setSpikeMonitor(MEC_LII_Stellate,"DEFAULT");
	sim->setConnectionMonitor(EC_LI_II_Multipolar_Pyramidal,MEC_LII_Stellate,"DEFAULT");
	// ---------------- RUN STATE -------------------
	sim->setExternalCurrent(EC_LI_II_Multipolar_Pyramidal, 1000);
	SpkMon->startRecording();
	for (int i=0; i<10; i++) {sim->runNetwork(0,100, true);}
	SpkMon->stopRecording();
	float test_presyn_cent_firing_rate = SpkMon->getPopMeanFiringRate();
	SpkMon->print(false);
	//printf("test_presyn_cent_firing_rate:%f\n",test_presyn_cent_firing_rate);	

#if CARLSIM_PRESYN_CENT_STDP
	// test that the firing rate with presynaptic centered stdp is less than that found with
	// symmetric stdp. The CARLSIM_PRESYN_CENT_STDP if statement causes this test to still pass with
	// symmetric stdp because the presynaptic centered stdp firing rate level is not checked.
	int symmetric_stdp_firing_rate = 854.0;
	EXPECT_LE(test_presyn_cent_firing_rate, symmetric_stdp_firing_rate); // ms
#endif

	delete sim;
}