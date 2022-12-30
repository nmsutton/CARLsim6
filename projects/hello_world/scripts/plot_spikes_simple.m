initOAT;
SR = SpikeReader('../../../.build/projects/CS6_conn_test_CSTP/results/spk_MEC_LII_Stellate.dat');
spikes = SR.readSpikes(1); X=[]; Y=[];
T=size(spikes,1); % time to plot
N=5;%size(spikes,2); % number of spikes to plot
for y=1:T 
for x=1:N
  if spikes(y,x)==1 X=[X;x]; Y=[Y;y]; end
end
end
scatter(Y,X,10);
title("Spike Times", 'FontSize', 11); axis('tight'); xlabel('Time'); ylabel('Neuron');