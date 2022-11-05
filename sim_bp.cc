#include "sim_bp.h"

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim bimodal 6 gcc_trace.txt
    argc = 4
    argv[0] = "sim"
    argv[1] = "bimodal"
    argv[2] = "6"
    ... and so on
*/
int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    bp_params params;       // look at sim_bp.h header file for the the definition of struct bp_params
    char outcome;           // Variable holds branch outcome
    unsigned long int addr; // Variable holds the address read from input file
    
    if (!(argc == 4 || argc == 5 || argc == 7))
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    params.bp_name  = argv[1];
    
    // strtoul() converts char* to unsigned long. It is included in <stdlib.h>
    if(strcmp(params.bp_name, "bimodal") == 0)              // Bimodal
    {
        if(argc != 4)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M2       = strtoul(argv[2], NULL, 10);
        trace_file      = argv[3];
        printf("COMMAND\n %s %s %lu %s\n", argv[0], params.bp_name, params.M2, trace_file);
        params.M1 = 0;
        params.K = 0;
        params.N = 0;
    }
    else if(strcmp(params.bp_name, "gshare") == 0)          // Gshare
    {
        if(argc != 5)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M1       = strtoul(argv[2], NULL, 10);
        params.N        = strtoul(argv[3], NULL, 10);
        trace_file      = argv[4];
        printf("COMMAND\n %s %s %lu %lu %s\n", argv[0], params.bp_name, params.M1, params.N, trace_file);
        params.M2 = 0;
        params.K = 0;
    }
    else if(strcmp(params.bp_name, "hybrid") == 0)          // Hybrid
    {
        if(argc != 7)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.K        = strtoul(argv[2], NULL, 10);
        params.M1       = strtoul(argv[3], NULL, 10);
        params.N        = strtoul(argv[4], NULL, 10);
        params.M2       = strtoul(argv[5], NULL, 10);
        trace_file      = argv[6];
        printf("COMMAND\n %s %s %lu %lu %lu %lu %s\n", argv[0], params.bp_name, params.K, params.M1, params.N, params.M2, trace_file);
    }
    else
    {
        printf("Error: Wrong branch predictor name:%s\n", params.bp_name);
        exit(EXIT_FAILURE);
    }
    
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }

    ulint index_value = 0;
    Bimodal BM(params.M2);
    Gshare GS(params.M1, params.N);
    Hybrid HY(params.K);
    
    char str[2];
    //ulint c = 0;
    while(fscanf(FP, "%lx %s", &addr, str) != EOF)
    {
        
        outcome = str[0];
        /*(if (outcome == 't')
            printf("=%lu\t%lx\t%s\n", c, addr, "t");           // Print and test if file is read correctly
        else if (outcome == 'n')
            printf("=%lu\t%lx\t%s\n", c, addr, "n");          // Print and test if file is read correctly*/
        /*************************************
            Add branch predictor code here
        **************************************/

        if(strcmp(params.bp_name, "bimodal") == 0)              // Bimodal
        {
            BM.active = 1;
            index_value = BM.get_index_bits(addr);
            BM.checkPrediction(index_value, outcome);
        }
        else if(strcmp(params.bp_name, "gshare") == 0)          // Gshare
        {
            GS.active = 1;
            index_value = GS.get_index_bits(addr);
            GS.checkPrediction(index_value, outcome);
        }
        else if(strcmp(params.bp_name, "hybrid") == 0)          // Hybrid
        {
            BM.active = 1;
            GS.active = 1;
            HY.active = 1;
            
            index_value = GS.get_index_bits(addr);
            char GS_prediction = GS.checkPrediction(index_value, outcome, true, false);
            index_value = BM.get_index_bits(addr);
            char BM_prediction = BM.checkPrediction(index_value, outcome, true, false);
            index_value = HY.get_index_bits(addr);
            char selected_predictor = HY.checkSelection(index_value, outcome, BM_prediction, GS_prediction, true, false);
            //std::cout << BM_prediction << " " << GS_prediction << " " << selected_predictor << "\n";
            if (selected_predictor == 'g')
            {
                index_value = GS.get_index_bits(addr);
                GS.checkPrediction(index_value, outcome, false, true);
            }
            else
            {
                index_value = BM.get_index_bits(addr);
                BM.checkPrediction(index_value, outcome, false, true);
                GS.updateBHR(GS_prediction, outcome);
            }
            index_value = HY.get_index_bits(addr);
            HY.checkSelection(index_value, outcome, BM_prediction, GS_prediction, false, true);
        }

        /*c++;

        if (c >= 10000)
        {
            break;
        }*/
    }

    if(strcmp(params.bp_name, "bimodal") == 0)              // Bimodal
    {
        BM.printOutput();
        BM.printPredictionHash();
    }
    else if(strcmp(params.bp_name, "gshare") == 0)          // Gshare
    {
        GS.printOutput();
        GS.printPredictionHash();
    }
    else if(strcmp(params.bp_name, "hybrid") == 0)          // Hybrid
    {
        HY.printOutput();
        HY.printPredictionHash();
        GS.printPredictionHash();
        BM.printPredictionHash();
    }

    return 0;
}
