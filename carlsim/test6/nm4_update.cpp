
#ifdef LN_I_CALC_TYPES

#include "gtest/gtest.h"
#include "carlsim_tests.h"

#include <carlsim.h>

//#define DEBUG_firingRateSTDvsSTFvsNM4

/*!
 * \brief Test the effect of the updateNM4Levels() on STDP
 * 
 * This test ensures that STDP processing is effected as expected with the updateNM4Levels() function.
 */
TEST(nm4update, firingRate) {
	// ---------------- CONFIG STATE -------------------
	LoggerMode logger = SILENT;  // USER, SILENT
	int numGPUs = 1;
	int randSeed = 42;	
	CARLsim* sim = new CARLsim("cstp_test_1conn", GPU_MODE, logger, numGPUs, randSeed);
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
	sim->setNeuromodulator(MEC_LII_Stellate, 
    1.0f, 1.0f, 1.0f, false, // DA
    1.0f, 1.0f, 1.0f, false, // 5HT
    1.0f, 1000000.0f, 0.000001f, true, // ACh
    1.0f, 1000000.0f, 0.000001f, true); // NE
	sim->setESTDP(EC_LI_II_Multipolar_Pyramidal, MEC_LII_Stellate, true, PKA_PLC_MOD, ExpCurve(0.1f, 20.0f, -0.1f, 20.0f), PkaPlcModulation(NM_NE, 1.0f, NM_ACh, 1.0f));
	sim->setWeightAndWeightChangeUpdate(INTERVAL_10MS, false, 0.5f);
	// ---------------- SETUP STATE -------------------
	sim->setupNetwork();
	SpikeMonitor* SpkMon = sim->setSpikeMonitor(MEC_LII_Stellate,"DEFAULT");
	sim->setConnectionMonitor(EC_LI_II_Multipolar_Pyramidal,MEC_LII_Stellate,"DEFAULT");
	// ---------------- RUN STATE -------------------
	sim->setExternalCurrent(EC_LI_II_Multipolar_Pyramidal, 1000);
	sim->updateNM4Levels(0, MEC_LII_Stellate, false, false, true, true, 0.0, 0.0, 1.0 /* ACh */, 0.0 /* NE */);
	SpkMon->startRecording();
	for (int i=0; i<10; i++) {sim->runNetwork(0,100, true);}
	SpkMon->stopRecording();
	float firing_rate_test_1 = SpkMon->getPopMeanFiringRate();
	SpkMon->print(false);
	//printf("firing_rate_test_1:%f\n",firing_rate_test_1);	

	delete sim;

	sim = new CARLsim("cstp_test_1conn", GPU_MODE, logger, numGPUs, randSeed);
	EC_LI_II_Multipolar_Pyramidal=sim->createGroup("EC_LI_II_Multipolar_Pyramidal", 
	                            5, EXCITATORY_NEURON, ANY, GPU_CORES);  
	MEC_LII_Stellate=sim->createGroup("MEC_LII_Stellate", 
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
	sim->setNeuromodulator(MEC_LII_Stellate, 
    1.0f, 1.0f, 1.0f, false, // DA
    1.0f, 1.0f, 1.0f, false, // 5HT
    1.0f, 1000000.0f, 0.000001f, true, // ACh
    1.0f, 1000000.0f, 0.000001f, true); // NE
	sim->setESTDP(EC_LI_II_Multipolar_Pyramidal, MEC_LII_Stellate, true, PKA_PLC_MOD, ExpCurve(0.1f, 20.0f, -0.1f, 20.0f), PkaPlcModulation(NM_NE, 1.0f, NM_ACh, 1.0f));
	sim->setWeightAndWeightChangeUpdate(INTERVAL_10MS, false, 0.5f);
	// ---------------- SETUP STATE -------------------
	sim->setupNetwork();
	SpkMon = sim->setSpikeMonitor(MEC_LII_Stellate,"DEFAULT");
	sim->setConnectionMonitor(EC_LI_II_Multipolar_Pyramidal,MEC_LII_Stellate,"DEFAULT");
	// ---------------- RUN STATE -------------------
	sim->setExternalCurrent(EC_LI_II_Multipolar_Pyramidal, 1000);
	sim->updateNM4Levels(0, MEC_LII_Stellate, false, false, true, true, 0.0, 0.0, 0.0 /* ACh */, 1.0 /* NE */);
	SpkMon->startRecording();
	for (int i=0; i<10; i++) {sim->runNetwork(0,100, true);}
	SpkMon->stopRecording();
	float firing_rate_test_2 = SpkMon->getPopMeanFiringRate();
	SpkMon->print(false);
	//printf("firing_rate_test_2:%f\n",firing_rate_test_2);	

	delete sim;

	// Check that ACh = 1 and NE = 0 creates a lower firing rate than Ach = 0 and NE = 1.
	EXPECT_LE(firing_rate_test_1, firing_rate_test_2); // ms
}

#endif