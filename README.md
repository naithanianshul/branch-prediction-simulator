# branch-prediction-simulator
A branch prediction simulator with bimodal, gshare and a hybrid predictor that selects between the bimodal and the gshare predictor using a chooser table of 2-bit counters.

To run the simulator first compile the code by executing the command 'make'.<br/>
The format of the command to run the cache simulation is:

* To run Bimodal Predictor -
>./sim<br/>
>&emsp;&emsp;<predictor_name = bimodal><br/>
>&emsp;&emsp;< M2 ><br/>
>&emsp;&emsp;<trace_file><br/>

Example - The cache specifications are:
* Number of PC bits used to index the bimodal table (M2) = 6
* Addresses to be accessed present in 'gcc_trace.txt' file

```./sim bimodal 6 gcc_trace.txt```

* To run Gshare Predictor -
>./sim<br/>
>&emsp;&emsp;<predictor_name = gshare><br/>
>&emsp;&emsp;< M1 ><br/>
>&emsp;&emsp;< N ><br/>
>&emsp;&emsp;<trace_file><br/>

Example - The cache specifications are:
* Number of PC bits (M1) = 9
* Global branch history register bits used to index the gshare table (N) = 3
* Addresses to be accessed present in 'gcc_trace.txt' file

```./sim gshare 9 3 gcc_trace.txt```

* To run Hybrid Predictor -
>./sim<br/>
>&emsp;&emsp;<predictor_name = hybrid><br/>
>&emsp;&emsp;< K ><br/>
>&emsp;&emsp;< M1 ><br/>
>&emsp;&emsp;< N ><br/>
>&emsp;&emsp;< M2 ><br/>
>&emsp;&emsp;<trace_file><br/>

Example - The cache specifications are:
* Number of PC bits used to index the chooser table (K) = 8
* Number of PC bits (M1) = 14
* Number of PC bits used to index the bimodal table (M2) = 10
* Global branch history register bits used to index the gshare table (N) = 5
* Addresses to be accessed present in 'gcc_trace.txt' file

```./sim hybrid 8 14 10 5 gcc_trace.txt```
