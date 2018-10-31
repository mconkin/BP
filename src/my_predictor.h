// my_predictor.h
// This file contains a sample my_predictor class.
// It is a simple 32,768-entry gshare with a history length of 15.
// Note that this predictor doesn't use the whole 32 kilobytes available
// for the CBP-2 contest; it is just an example.

class my_update : public branch_update {
public:
	unsigned int gshare_index;
	unsigned int local_index;
};

class my_predictor : public branch_predictor {
public:
#define HISTORY_LENGTH	15
#define TABLE_BITS	15

	my_update u;
	branch_info bi;

	unsigned int history;
	unsigned char tournament;

	unsigned char gshare[1<<TABLE_BITS];
	unsigned char gshare_prediction;

	unsigned char local[1<<TABLE_BITS];
	unsigned char local_prediction;

	my_predictor (void) : history(0), tournament(2) { 
		memset (gshare, 0, sizeof (gshare));
		memset (local, 0, sizeof (local));
	}

	branch_update *predict (branch_info & b) {
		
		bi = b;
		if (b.br_flags & BR_CONDITIONAL) {

			// compute gshare
			u.gshare_index = 
				  (history << (TABLE_BITS - HISTORY_LENGTH)) 
				^ (b.address & ((1<<TABLE_BITS)-1));
			gshare_prediction = gshare[u.gshare_index] >> 1;
			
			// compute local
			u.local_index = 
				  (b.address & ((1<<TABLE_BITS)-1));
			local_prediction = local[u.local_index] >> 1;

		
			// choose the predictor
			if (tournament >> 1) {
				u.direction_prediction (gshare_prediction);
			} else {
				u.direction_prediction (local_prediction);
			}
		} else {
			u.direction_prediction (true);
		}
		u.target_prediction (0);
		return &u;
	}

	void update (branch_update *u, bool taken, unsigned int target) {
		if (bi.br_flags & BR_CONDITIONAL) {

			// update ghare
			unsigned char *c = &gshare[((my_update*)u)->gshare_index];
			if (taken) {
				if (*c < 3) (*c)++;
			} else {
				if (*c > 0) (*c)--;
			}
			history <<= 1;
			history |= taken;
			history &= (1<<HISTORY_LENGTH)-1;

			//update local 
			c = &local[((my_update*)u)->local_index];
			if (taken) {
				if (*c < 3) (*c)++;
			} else {
				if (*c > 0) (*c)--;
			}

			//update tournament
			if (taken != (bool) local_prediction 
				&& taken == (bool) gshare_prediction) {
				
				if (tournament < 3) (tournament)++;
			}
			 else if (taken == (bool) local_prediction 
				&& taken != (bool) gshare_prediction) {
				
				if (tournament > 0) (tournament)--;
			}
		}
	}
};
