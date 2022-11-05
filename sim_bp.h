#ifndef SIM_BP_H
#define SIM_BP_H
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <cstring>
#include <string.h>
#include <bitset>

typedef struct bp_params{
    unsigned long int K;
    unsigned long int M1;
    unsigned long int M2;
    unsigned long int N;
    char*             bp_name;
}bp_params;

typedef unsigned long int ulint;

// Put additional data structures here as per your requirement
void print(std::string text = "", std::string caption = ":")
{
	if (true)
	{
		if (caption == ":")
		{
			std::cout << text << '\n';
		}
		else
		{
			std::cout << caption << ": " << text << '\n';
		}
	}
}
void print(ulint text, std::string caption = ":")
{
	if (true)
	{
		if (caption == ":")
		{
			std::cout << text << '\n';
		}
		else
		{
			std::cout << caption << ": " << text << '\n';
		}
	}
}

std::string hex2bin(ulint number)
{
    std::bitset<32> bits;
	bits = number;
	std::string addressBinary = bits.to_string();
    return addressBinary;
}

std::string int2str(ulint numbers)
{
	std::string text = std::to_string(numbers);
	return text;
}

std::string dec2hex(ulint dec_num)
{
	std::stringstream s;
	s << std::hex << dec_num;
	std::string hex_num ( s.str() );
	return hex_num;
}

ulint hex2dec(std::string hex_num)
{
	ulint dec_num;
	std::stringstream s;
    s << hex_num;
    s >> std::hex >> dec_num;
	return dec_num;
}

ulint bin2dec(std::string bin_num)
{
    ulint n = bin_num.length();
    char c[n + 1];
    std::strcpy(c, bin_num.c_str());
	ulint dec_num = std::strtoll(c, nullptr, 2);
	return dec_num;
}

std::string bin2hex(std::string bin_num)
{
	ulint dec_num = bin2dec(bin_num);
	return dec2hex(dec_num);
}

class Bimodal
{
    public:
    bool active;

    ulint no_of_pc_bits;
    ulint total_sets;

    ulint total_predictions;
    ulint no_of_mispredictions;

    std::map<ulint, int> prediction_hash;

    Bimodal (ulint no_of_pc_bits)
    {
        this->no_of_pc_bits = 0;
        this->total_predictions = 0;
        this->no_of_mispredictions = 0;
        
        this->no_of_pc_bits = no_of_pc_bits;

        this->active = 0;

        this->total_sets = pow( 2, this->no_of_pc_bits );
        for (ulint i = 0; i < total_sets; i++)
        {
            prediction_hash[i] = 2;
        }
    }

    ulint get_index_bits(ulint addr)
    {
        std::string addr_binary = hex2bin(addr);
        std::string index_bits = "";
        for (ulint i = 0; i < this->no_of_pc_bits; i++)
        {
            index_bits = index_bits + addr_binary[(32 - this->no_of_pc_bits - 2)+i];
        }
        return bin2dec(index_bits);
    }

    void actualTaken(ulint index_value)
    {
        (prediction_hash[index_value] < 3 ) ? prediction_hash[index_value] ++ : NULL;
        //printf("\tBU: %lu\t%d\n", index_value, prediction_hash[index_value]);
    }

    void actualNotTaken(ulint index_value)
    {
        (prediction_hash[index_value] > 0 ) ? prediction_hash[index_value] -- : NULL;
        //printf("\tBU: %lu\t%d\n", index_value, prediction_hash[index_value]);
    }

    char getPrediction(ulint index_value, bool print_statement = true)
    {
        char prediction = ' ';
        //(print_statement == true) ? printf("\tBP: %lu\t%d\n", index_value, prediction_hash[index_value]) : NULL;
        prediction = (prediction_hash[index_value] >= 2) ? 't' : 'n';
        return prediction;
    }

    char checkPrediction(ulint index_value, char actual_outcome, bool fetch_prediction = true, bool update_state = true)
    {
        char predicted_outcome = predicted_outcome = getPrediction(index_value, fetch_prediction);
        if (fetch_prediction == true)
        {
            this->total_predictions ++;
            if (predicted_outcome != actual_outcome)            // Wrong Prediction
            {
                this->no_of_mispredictions ++;
            }
        }

        if (update_state == true)
        {
            (actual_outcome == 't') ? actualTaken(index_value) : actualNotTaken(index_value);
        }

        return predicted_outcome;
    }

    void printOutput()
    {
        if (this->active == true)
        {
            printf("OUTPUT\n");
            printf(" number of predictions:    %lu\n", this->total_predictions);
            printf(" number of mispredictions: %lu\n", this->no_of_mispredictions);
            std::cout << " misprediction rate:       " << std::fixed << std::setprecision(2) <<((float)this->no_of_mispredictions * 100)/(float)this->total_predictions << "%\n";
        }
    }

    void printPredictionHash()
    {
        if (this->active == true)
        {
            printf("FINAL BIMODAL CONTENTS\n");
            for (ulint i = 0; i < this->total_sets; i++)
            {
                printf(" %lu\t%d\n",i,prediction_hash[i]);
            }
        }
    }
};

class Gshare
{
    public:
    bool active;

    ulint no_of_pc_bits;
    ulint no_of_gh_bits;
    ulint total_sets;

    ulint total_predictions;
    ulint no_of_mispredictions;

    std::map<ulint, int> prediction_hash;
    std::string BHR;

    Gshare (ulint no_of_pc_bits, ulint no_of_gh_bits)
    {
        this->no_of_pc_bits = 0;
        this->no_of_gh_bits = 0;
        this->total_predictions = 0;
        this->no_of_mispredictions = 0;
        
        this->no_of_pc_bits = no_of_pc_bits;
        this->no_of_gh_bits = no_of_gh_bits;

        this->active = 0;

        this->BHR = "";
        for (ulint i = 0; i < this->no_of_gh_bits; i++)
        {
            this->BHR += "0";
        }

        this->total_sets = pow( 2, this->no_of_pc_bits );
        for (ulint i = 0; i < this->total_sets; i++)
        {
            prediction_hash[i] = 2;
        }
    }

    ulint get_index_bits(ulint addr)
    {
        std::string addr_binary = hex2bin(addr);
        std::string address_bits = "";
        std::string n_address_bits = "";
        std::string m_minus_n_address_bits = "";
        for (ulint i = 0; i < this->no_of_pc_bits; i++)
        {
            address_bits = address_bits + addr_binary[(32 - this->no_of_pc_bits - 2)+i];
        }
        //std::cout << "Address=" << address_bits << '\n';

        for (ulint i = 0; i < this->no_of_gh_bits; i++)
        {
            n_address_bits = n_address_bits + address_bits[i];
        }
        
        std::string index_p1 = "";
        for (ulint i = 0; i < this->no_of_gh_bits; i++)
        {
            address_bits.erase(address_bits.begin());
            //std::cout << "Bit " << BHR[i] << '\n';
            if (n_address_bits[i] == this->BHR[i])
            {
                index_p1 += '0';
            }
            else
            {
                index_p1 += '1';
            }
        }
        //std::cout << "First N bits of Address=" << n_address_bits << " and BHR=" << this->BHR << " and xor=" << index_p1 <<'\n';

        ulint index_dec = bin2dec(index_p1 + address_bits);
        //std::cout << "Index bin=" << index_p1 + address_bits << " and Index dec=" << index_dec << '\n';

        //std::cout << index_p1 + address_bits << " " << index_dec << '\n';
        return index_dec;
    }

    void updateBHR(char predicted_outcome,char actual_outcome)
    {
        //std::cout << BHR << "\n";
        this->BHR.erase(this->BHR.length()-1);
        (actual_outcome == 't') ? this->BHR = '1'+this->BHR: this->BHR = '0'+this->BHR;
    }
    
    void actualTaken(ulint index_value)
    {
        (prediction_hash[index_value] < 3 ) ? prediction_hash[index_value] ++ : NULL;
        //printf("\tGU: %lu\t%d\n", index_value, prediction_hash[index_value]);
    }

    void actualNotTaken(ulint index_value)
    {
        (prediction_hash[index_value] > 0 ) ? prediction_hash[index_value] -- : NULL;
        //printf("\tGU: %lu\t%d\n", index_value, prediction_hash[index_value]);
    }

    char getPrediction(ulint index_value, bool print_statement = true)
    {
        char prediction = ' ';
        //(print_statement == true) ? printf("\tGP: %lu\t%d\n", index_value, prediction_hash[index_value]) : NULL;
        //std::cout << "Prediction=" << prediction_hash[index_value];
        prediction = (prediction_hash[index_value] >= 2) ? 't' : 'n';
        return prediction;
    }

    char checkPrediction(ulint index_value, char actual_outcome, bool fetch_prediction = true, bool update_state = true)
    {
        char predicted_outcome = predicted_outcome = getPrediction(index_value, fetch_prediction);
        //std::cout << " and Outcome=" << actual_outcome << '\n' ;
        if (fetch_prediction == true)
        {
            this->total_predictions ++;
            if (predicted_outcome != actual_outcome)            // Wrong Prediction
            {
                this->no_of_mispredictions ++;
            }
        }
        
        if (update_state == true)
        {
            updateBHR(predicted_outcome, actual_outcome);
            (actual_outcome == 't') ? actualTaken(index_value) : actualNotTaken(index_value);
        }

        return predicted_outcome;
    }

    void printOutput()
    {
        if (this->active == true)
        {
            printf("OUTPUT\n");
            printf(" number of predictions:    %lu\n", this->total_predictions);
            printf(" number of mispredictions: %lu\n", this->no_of_mispredictions);
            std::cout << " misprediction rate:       " << std::fixed << std::setprecision(2) <<((float)this->no_of_mispredictions * 100)/(float)this->total_predictions << "%\n";
        }
    }

    void printPredictionHash()
    {
        if (this->active == true)
        {
            printf("FINAL GSHARE CONTENTS\n");
            for (ulint i = 0; i < this->total_sets; i++)
            {
                printf(" %lu\t%d\n",i,prediction_hash[i]);
            }
        }
    }
};

class Hybrid
{
    public:
    bool active;
    ulint no_of_pc_bits;
    ulint total_sets;

    ulint total_predictions;
    ulint no_of_mispredictions;

    std::map<ulint, int> selection_hash;

    Hybrid(ulint no_of_pc_bits)
    {
        this->no_of_pc_bits = no_of_pc_bits;
        this->total_sets = pow( 2, this->no_of_pc_bits );

        this->total_predictions = 0;
        this->no_of_mispredictions = 0;

        for (ulint i = 0; i < this->total_sets; i++)
        {
            selection_hash[i] = 1;
        }
    }

    ulint get_index_bits(ulint addr)
    {
        std::string addr_binary = hex2bin(addr);
        std::string index_bits = "";
        for (ulint i = 0; i < this->no_of_pc_bits; i++)
        {
            index_bits = index_bits + addr_binary[(32 - this->no_of_pc_bits - 2)+i];
        }

        //std::cout << "Index binary = " << index_bits << " and dec = " << bin2dec(index_bits) << '\n';

        return bin2dec(index_bits);
    }

    char getSelection(ulint index_value, bool print_statement = true)
    {
        char selection = ' ';
        //(print_statement == true) ? printf("\tCP: %lu\t%d\n", index_value, selection_hash[index_value]) : NULL;
        selection = (selection_hash[index_value] >= 2) ? 'g' : 'b';
        return selection;
    }

    void bimodalCorrect(ulint index_value)
    {
        (selection_hash[index_value] > 0 ) ? selection_hash[index_value] -- : NULL;
        //printf("\tCU: %lu\t%d\n", index_value, selection_hash[index_value]);
    }

    void gshareCorrect(ulint index_value)
    {
        (selection_hash[index_value] < 3 ) ? selection_hash[index_value] ++ : NULL;
        //printf("\tCU: %lu\t%d\n", index_value, selection_hash[index_value]);
    }

    char checkSelection(ulint index_value, char actual_prediction, char BM_prediction, char GS_prediction, bool fetch_selection = true, bool update_state = true)
    {
        //printf("actual=%c bm=%c gs=%c\n", actual_prediction, BM_prediction, GS_prediction);
        char predicted_selection = getSelection(index_value, fetch_selection);
        //printf("predictor is %c\n", predicted_selection);
        if (fetch_selection == true)
        {
            this->total_predictions ++;
        }
        
        if (update_state == true)
        {
            if ( (BM_prediction == actual_prediction) && (GS_prediction != actual_prediction) )
            {
                bimodalCorrect(index_value);
            }
            else if ( (BM_prediction != actual_prediction) && (GS_prediction == actual_prediction) )
            {
                gshareCorrect(index_value);
            }

            if ( (predicted_selection == 'b') && (BM_prediction != actual_prediction) )
            {
                this->no_of_mispredictions++;
            }
            if ( (predicted_selection == 'g') && (GS_prediction != actual_prediction) )
            {
                this->no_of_mispredictions++;
            }
            //std::cout << BM_prediction << '\n';
        }
        
        return predicted_selection;
    }

    void printOutput()
    {
        if (this->active == true)
        {
            printf("OUTPUT\n");
            printf(" number of predictions:    %lu\n", this->total_predictions);
            printf(" number of mispredictions: %lu\n", this->no_of_mispredictions);
            std::cout << " misprediction rate:       " << std::fixed << std::setprecision(2) <<((float)this->no_of_mispredictions * 100)/(float)this->total_predictions << "%\n";
        }
    }

    void printPredictionHash()
    {
        if (this->active == true)
        {
            printf("FINAL CHOOSER CONTENTS\n");
            for (ulint i = 0; i < this->total_sets; i++)
            {
                printf(" %lu\t%d\n",i,selection_hash[i]);
            }
        }
    }
};

#endif
