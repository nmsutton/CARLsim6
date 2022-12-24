/* * Copyright (c) 2016 Regents of the University of California. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* 3. The names of its contributors may not be used to endorse or promote
*    products derived from this software without specific prior written
*    permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* *********************************************************************************************** *
* CARLsim
* created by: (MDR) Micah Richert, (JN) Jayram M. Nageswaran
* maintained by:
* (MA) Mike Avery <averym@uci.edu>
* (MB) Michael Beyeler <mbeyeler@uci.edu>,
* (KDC) Kristofor Carlson <kdcarlso@uci.edu>
* (TSC) Ting-Shuo Chou <tingshuc@uci.edu>
* (HK) Hirak J Kashyap <kashyaph@uci.edu>
*
* CARLsim v1.0: JM, MDR
* CARLsim v2.0/v2.1/v2.2: JM, MDR, MA, MB, KDC
* CARLsim3: MB, KDC, TSC
* CARLsim4: TSC, HK
* CARLsim5: HK, JX, KC
* CARLsim6: LN, JX, KC, KW
*
* CARLsim available from http://socsci.uci.edu/~jkrichma/CARLsim/
* Ver 12/31/2016
*/

// include CARLsim user interface
#include <carlsim.h>

// Include libraries that will allow for us to perform vector operations, and
// print their results
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <boost/iterator/counting_iterator.hpp>
#include <ctime>
#include <cstdlib>
#include <unordered_map>

int main() {
	// ---------------- CONFIG STATE -------------------
	int numGPUs = 1;
	int randSeed = 42;
	CARLsim sim("hello world", GPU_MODE, USER, numGPUs, randSeed);
	#include "../generate_config_state.cpp" 	// configure the network
	// ---------------- SETUP STATE -------------------
	sim.setupNetwork();

	int pyr_start = sim.getGroupStartNeuronId(EC_LI_II_Multipolar_Pyramidal);
	int pyr_end = sim.getGroupEndNeuronId(EC_LI_II_Multipolar_Pyramidal);
	int pyr_range = (pyr_end - pyr_start) + 1;

    std::vector<int> pyr_vec( boost::counting_iterator<int>( 0 ),
                              boost::counting_iterator<int>( pyr_range ));
    std::vector<float> pc_background_current(pyr_range, 0.0f);
    std::vector<float> pc_current = pc_background_current;
    
	SpikeMonitor* SpkMon = sim.setSpikeMonitor(MEC_LII_Stellate,"DEFAULT");
	// ---------------- RUN STATE -------------------
	// sim.setExternalCurrent(EC_LI_II_Multipolar_Pyramidal, 100);
	SpkMon->startRecording();
    srand48(3);
	for (int i=0; i<10000; i++) 
    {
            float pc_Iapp = 50.0f;
            float pc_Iapp_sd = 75.0f;
            for (int i = 0; i < pyr_range; i++)
            {
                pc_background_current.at(i) = pc_Iapp + pc_Iapp_sd * (float)drand48();
            }   
            sim.setExternalCurrent(EC_LI_II_Multipolar_Pyramidal, pc_background_current);
            sim.runNetwork(0,1);
                
    }        
	// for (int i=0; i<10; i++) {sim.runNetwork(0,100, true);}
	SpkMon->stopRecording();
	SpkMon->print(false);
	return 0;
}
