#include <iostream>
#include <string>
#include <Windows.h>
#include <algorithm>
#include <cstddef>		// req for size_t
#include <vector>
#include <cctype>
#include <atomic>
#include <thread>
#include <chrono>
#include <unordered_set>
#include <fstream>		// file handling
#include <iomanip>		// format time and date
#include <ctime>		// get current time
#include <cstdlib>		// clearscreen
#include <filesystem>	// use c++17 compat

//#include "alphabs.h"
//#include "presets.h"

using namespace std;

class Tabula {
public:
	string key;
	string alpha;

	// use Kryptos as defaults, useful!!

	Tabula(string key, string alpha = "KRYPTOSABCDEFGHIJLMNQUVWXZ") {
		setKey(key);
		setAlpha(alpha);
	}

	/*SET UPPERCASE VAR*/

	void setKey(string key) {
		this->key = "";
		for (char c : key) {
			if (isalpha(c)) {
				this->key += toupper(c);
			}
		}
	}

	void setAlpha(string alpha) {
		this->alpha = "";
		for (char c : alpha) {
			if (isalpha(c)) {
				this->alpha += toupper(c);
			}
		}
	}

	/*MAIN ENC/DEC LOGIC*/

	string e_c(string text) {	// ### ENCRYPTION ###
		string out;
		size_t keyLength = key.length();
		size_t alphaLength = alpha.length();

		for (int i = 0, j = 0; i < text.length(); ++i) {
			char c = toupper(text[i]);
			size_t textPos = alpha.find(c);

			if (textPos != string::npos) {
				size_t keyPos = alpha.find(key[j % keyLength]);
				out += alpha[(textPos + keyPos) % alphaLength];
				j++;
			}
			else {
				out += c; // non-alpha chars
			}
		}

		return out;
	}

	string d_c(string text) {	// $$$ DECRYPTION $$$
		string out;
		size_t keyLength = key.length();
		size_t alphaLength = alpha.length();

		for (int i = 0, j = 0; i < text.length(); ++i) {
			char c = toupper(text[i]);
			size_t textPos = alpha.find(c);

			if (textPos != string::npos) {
				size_t keyPos = alpha.find(key[j % keyLength]);
				out += alpha[(textPos - keyPos + alphaLength) % alphaLength];
				j++;
			}
			else {
				out += c; // non-alpha chars added
			}
		}

		return out;
	}
};

/*FUNC FOR KEYED ALPHABET STRINGS BY COMMAND OR PROXY*/

string setKeyAlpha(const string& use_key) {
	unordered_set<char> used_chars;
	string upperKey;
	string keyAlpha;

	for (char c : use_key) {
		char upper_c = toupper(c);
		if (isalpha(upper_c) && used_chars.find(upper_c) == used_chars.end()) {
			keyAlpha.push_back(upper_c);
			used_chars.insert(upper_c);
		}
	}

	for (char c = 'A'; c <= 'Z'; ++c) {
		if (used_chars.find(c) == used_chars.end()) {
			keyAlpha.push_back(c);
		}
	}

	return keyAlpha;
}

void rev_Str(string& str_R) {
	if (str_R.length() < 2) {
		return;
	}

	size_t lleft = 0;
	size_t rright = str_R.length() - 1;

	while (lleft < rright) {
		swap(str_R[lleft], str_R[rright]);
		++lleft;
		--rright;
	}
}

void gen_tabl(const string& alpha_b) {
	size_t size = alpha_b.length();

	return;
}


string list_mod1(const string& input) {
	string result;
	bool first = true;

	for (char c : input) {
		if (!isalpha(c)) continue;
		if (!first) {
			result += " ";
		}
		first = false;
		int val_mod1 = toupper(c) - 'A' + 1;
		result += to_string(val_mod1);
	}

	return result;
}

string list_mod0(const string& input) {
	string result;
	bool first = true;

	for (char c : input) {
		if (!isalpha(c)) continue;
		if (!first) {
			result += " ";
		}
		first = false;
		int val_mod0 = toupper(c) - 'A';
		result += to_string(val_mod0);
	}

	return result;
}

int let_mod1(char c) {
	return toupper(c) - 'A' + 1;
}

int let_mod0(char c) {
	return toupper(c) - 'A';
}

/* ------------------------ */
//							//
/* LOGIC IS ABOVE THIS LINE */
//							//
/* ------------------------ */

/*GET DATE FOR PRINT FUNC*/

string getCurrentDateTime() { // function to get time and date
	time_t now = time(0);
	tm localTime; // = localtime_s(&now);

	if (localtime_s(&localTime, &now) != 0) {
		cerr << "~ ~ ~ error -- time | failure of time conversion";
		return "";
	}

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%Y_%m_%d_%H_%M_%S", &localTime);
	return string(buffer);
}

/*SYSTEM CALL FOR CLS*/

void clearScreen() { // funct to cls
	system("cls"); // windows based
}

/*CREATE SUBFOLDER IN APP DIR FOR PRINTING FILES ONE AT A TIME (BEST PRACTICE)*/

//int print_V = 0;

//atomic<bool> print_Val(true);

/*void savesprinting() {
	print_Val = true;
}

void doesnotprint() {
	print_Val = false;
}*/

void saveStateToFile(const string& hash, const string& key, const string& fetchKey, const string& alpha, const string& e_c, const string& d_c) {
	string subfolder = "8BAL_txt"; // subfolder dir
	if (!filesystem::exists(subfolder)) {
		filesystem::create_directory(subfolder); // create one if absent
	}

	string filename = subfolder + "/" + key + "_" + getCurrentDateTime() + ".txt"; // gen filename

	ofstream outFile(filename);
	if (outFile.is_open()) {
		outFile << "  > > [c] i p h e r : \n\n\n" << hash << "\n\n\n";
		outFile << "  > >  k e y [w] o r d : \n\n\n" << key << "\n\n\n";
		outFile << "  > >  w o r d [s] h i f t : \n\n\n" << fetchKey << "\n\n\n";
		outFile << "  > > [a] l p h a b e t : \n\n\n" << alpha << "\n\n\n";
		outFile << "  > >  p h r a s e  [e] n c r y p t : \n\n\n" << e_c << "\n\n\n";
		outFile << "  > >  p h r a s e  [d] e c r y p t : \n\n\n" << d_c << "\n\n\n";
		outFile.flush();
		outFile.close();
	}
	/*else if (!outFile.is_open()) { // no lie just toss this idea
		cout << "[Print]\t\tPrinting is OFF! | Type '~pon' to turn on\n";
	}*/
}
	


/* CALL FUNC BY NAME SEPERATELY */

void call_print() {
	cout << "[Printed File!]\n";
}

void call_title() {
	cout << "\n\n";
	cout << "\t   ....................................................  \n";
	cout << "\t  [   |#########|  |########|     |#####|   |##|       ] \n";
	cout << "\t  ]  |##|     |##| |##|    |##|  |##| |##|  |##|       [ \n";
	cout << "\t  [  |##|     |##| |##|    |##| |##|   |##| |##|       ] \n";
	cout << "\t  ]   |#########|  |#########|  |##|   |##| |##|       [ \n";
	cout << "\t  [  |##|     |##| |##|    |##| |#########| |##|       ] \n";
	cout << "\t  ]  |##|     |##| |##|    |##| |##|   |##| |##|       [ \n";
	cout << "\t  [   |#########|  |#########|  |##|   |##| |########| ] \n";
	cout << "\t   ````````````````````````````````````````````````````  \n";
	cout << "\n\n";


}

void call_funcs() {
	cout << "\n";
	cout << "\t\t [main funcs]\n";
	cout << "\t(~)\t(!)\t(@)\t(#)\n";
	cout << "\t(a)\t(.)\t(s)\t(:)\n";
	cout << "\t(q)\t(e)\t(d)\t(z)\n";
	cout << "\t(xc)\t(~cls)\t(~help)\t(~reset)\n";
	cout << "\t(^)\t(*)\t\n";
	cout << "\n";
	cout << "\n";
}

void call_manual() {
	cout << "\n";
	cout << "\t\t [main funcs]\n";
	cout << "\t(~)\tfor other Commands\t\n";
	cout << "\t(~cls)\tClearsreen\n\t(~help)\n\t(~rst)\tReset\n";
	cout << "\t(!)\tset Keyword\t\t\n";
	cout << "\t(@)\tset Alphabet\n";
	cout << "\t(#)\tset Cipher\n";
	cout << "\t(a)\tshow Alphabet\n";
	cout << "\t(.)\tto set a Keyed Alphabet\n";
	cout << "\t(s)\tshow Shiftword\n";
	cout << "\t(q)\tall Values\n\t(e)\tEncrypt\n\t(d)\tDecrypt\n\t(z)\tEnc/Dec\n\t(xc)\tshow States\n";
	cout << "\t(:)\tfor Presets\n";
	cout << "\t(^)\twill reverse text specified\n";
	cout << "\t(*)\tdoes conversion values\n";
	cout << "\t\n";
	cout << "\n";
	cout << "\n";
}

void call_tips() {
	cout << "\t(!) \n";
	cout << "\t(@) \n";
	cout << "\t(#) \n";
	cout << "\t($)...\n";
	cout << "\t\n";
}

void call_defaults() {
	cout << "\n";
	cout << "\t\t[Default values]\n";
	cout << "\t-- (a)lphabet\t| KRYPTOSABCDEFGHIJLMNQUVWXZ\n";
	cout << "\t-- key(w)ord\t| PSITSASSIMPLEASABC\n";
	cout << "\t-- (c)ipher\t| OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAROBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR\n\n";
}

void call_version() {
	cout << "\n";
	cout << "\t\t[Version]\n" << "20250404C|TGB" << "\n";
}

void call_quit() {
	cout << "\n\n\n";
	cout << "[Quitting...]\t\t\tS A I N U U L A R A !\n";
}

int main() {

	SetConsoleTitle(L"8BAL");

	/*THIS IS JUST THE ANTIPASTA*/

	/*BEYOND THIS POINT IS MEAT & POTATOES*/

	/*set defaults, introduce the base logic*/

	Tabula cipher("PSITSASSIMPLEASABC", "KRYPTOSABCDEFGHIJLMNQUVWXZ");
	string hash = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";

	string keyAlpha = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
	string fetchKey = "KRYPTOS";

	// presets for the KRYPTOS cipher

	// the full text 
	string pre_k0 = "EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFDVFPJUDEEHZWETZYVGWHKKQETGFQJNCEGGWHKK?DQMCPFQZDQMMIAGPFXHQRLGTIMVMZJANQLVKQEDAGDVFRPJUNGEUNAQZGZLECGYUXUEENJTBJLBQCRTBJDFHRRYIZETKZEMVDUFKSJHKFWHKUWQLSZFTIHHDDDUVH?DWKBFUFPWNTDFIYCUQZEREEVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDXFLGGTEZ?FKZBSFDQVGOGIPUFXHHDRKFFHQNTGPUAECNUVPDJMQCLQUMUNEDFQELZZVRRGKFFVOEEXBDMVPNFQXEZLGREDNQFMPNZGLFLPMRJQYALMGNUVPDXVKPDQUMEBEDMHDAFMJGZNUPLGEWJLLAETGENDYAHROHNLSRHEOCPTEOIBIDYSHNAIACHTNREYULDSLLSLLNOHSNOSMRWXMNETPRNGATIHNRARPESLNNELEBLPIIACAEWMTWNDITEENRAHCTENEUDRETNHAEOETFOLSEDTIWENHAEIOYTEYQHEENCTAYCREIFTBRSPAMHHEWENATAMATEGYEERLBTEEFOASFIOTUETUAEOTOARMAEERTNRTIBSEDDNIAAHTTMSTEWPIEROAGRIEWFEBAECTDDHILCEIHSITEGOEAOSDDRYDLORITRKLMLEHAGTDHARDPNEOHMGFMFEUHEECDMRIPFEIMEHNLSSTTRTVDOHW?OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";

	string pre_k1 = "EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFD"; /* store a copy!
	string pre_k1 = "EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFD";*/
	string pre_k2 = "VFPJUDEEHZWETZYVGWHKKQETGFQJNCEGGWHKK?DQMCPFQZDQMMIAGPFXHQRLGTIMVMZJANQLVKQEDAGDVFRPJUNGEUNAQZGZLECGYUXUEENJTBJLBQCRTBJDFHRRYIZETKZEMVDUFKSJHKFWHKUWQLSZFTIHHDDDUVH?DWKBFUFPWNTDFIYCUQZEREEVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDXFLGGTEZ?FKZBSFDQVGOGIPUFXHHDRKFFHQNTGPUAECNUVPDJMQCLQUMUNEDFQELZZVRRGKFFVOEEXBDMVPNFQXEZLGREDNQFMPNZGLFLPMRJQYALMGNUVPDXVKPDQUMEBEDMHDAFMJGZNUPLGEWJLLAETG";/*
	string pre_k2 = "VFPJUDEEHZWETZYVGWHKKQETGFQJNCEGGWHKK?DQMCPFQZDQMMIAGPFXHQRLGTIMVMZJANQLVKQEDAGDVFRPJUNGEUNAQZGZLECGYUXUEENJTBJLBQCRTBJDFHRRYIZETKZEMVDUFKSJHKFWHKUWQLSZFTIHHDDDUVH?DWKBFUFPWNTDFIYCUQZEREEVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDXFLGGTEZ?FKZBSFDQVGOGIPUFXHHDRKFFHQNTGPUAECNUVPDJMQCLQUMUNEDFQELZZVRRGKFFVOEEXBDMVPNFQXEZLGREDNQFMPNZGLFLPMRJQYALMGNUVPDXVKPDQUMEBEDMHDAFMJGZNUPLGEWJLLAETG";*/
	string pre_k3 = "ENDYAHROHNLSRHEOCPTEOIBIDYSHNAIACHTNREYULDSLLSLLNOHSNOSMRWXMNETPRNGATIHNRARPESLNNELEBLPIIACAEWMTWNDITEENRAHCTENEUDRETNHAEOETFOLSEDTIWENHAEIOYTEYQHEENCTAYCREIFTBRSPAMHHEWENATAMATEGYEERLBTEEFOASFIOTUETUAEOTOARMAEERTNRTIBSEDDNIAAHTTMSTEWPIEROAGRIEWFEBAECTDDHILCEIHSITEGOEAOSDDRYDLORITRKLMLEHAGTDHARDPNEOHMGFMFEUHEECDMRIPFEIMEHNLSSTTRTVDOHW?";/*
	string pre_k3 = "ENDYAHROHNLSRHEOCPTEOIBIDYSHNAIACHTNREYULDSLLSLLNOHSNOSMRWXMNETPRNGATIHNRARPESLNNELEBLPIIACAEWMTWNDITEENRAHCTENEUDRETNHAEOETFOLSEDTIWENHAEIOYTEYQHEENCTAYCREIFTBRSPAMHHEWENATAMATEGYEERLBTEEFOASFIOTUETUAEOTOARMAEERTNRTIBSEDDNIAAHTTMSTEWPIEROAGRIEWFEBAECTDDHILCEIHSITEGOEAOSDDRYDLORITRKLMLEHAGTDHARDPNEOHMGFMFEUHEECDMRIPFEIMEHNLSSTTRTVDOHW?";*/
	string pre_k4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";/*
	string pre_k4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";*/
	
	string pre_k5 = "DOESLANGLEYKNOWABOUTTHIS?"; // there may be more!?

	string pre_s0 = "BETWEENSUBTLESHADINGANDTHEABSENCEOFLIGHTLIESTHENUANCEOFIQLUSIONITWASTOTALLYINVISIBLEHOWSTHATPOSSIBLE?THEYUSEDTHEEARTHSMAGNETICFIELDXTHEINFORMATIONWASGATHEREDANDTRANSMITTEDUNDERGRUUNDTOANUNKNOWNLOCATIONXDOESLANGLEYKNOWABOUTTHIS?THEYSHOULDITSBURIEDOUTTHERESOMEWHEREXWHOKNOWSTHEEXACTLOCATION?ONLYWWTHISWASHISLASTMESSAGEXTHIRTYEIGHTDEGREESFIFTYSEVENMINUTESSIXPOINTFIVESECONDSNORTHSEVENTYSEVENDEGREESEIGHTMINUTESFORTYFOURSECONDSWESTIDBYROWSSLOWLYDESPARATLYSLOWLYTHEREMAINSOFPASSAGEDEBRISTHATENCUMBEREDTHELOWERPARTOFTHEDOORWAYWASREMOVEDWITHTREMBLINGHANDSIMADEATINYBREACHINTHEUPPERLEFTHANDCORNERANDTHENWIDENINGTHEHOLEALITTLEIINSERTEDTHECANDLEANDPEEREDINTHEHOTAIRESCAPINGFROMTHECHAMBERCAUSEDTHEFLAMETOFLICKERBUTPRESENTLYDETAILSOFTHEROOMWITHINEMERGEDFROMTHEMISTXCANYOUSEEANYTHINGQ?OBKRUOXOGHULBSOLIFBBWEASTNORTHEASTOTWTQSJQSSEKZZWATJKLUDIAWINFBBERLINCLOCKWGDKZXTJCDIGKUHUAUEKCAR";

	string pre_s1 = "BETWEENSUBTLESHADINGANDTHEABSENCEOFLIGHTLIESTHENUANCEOFIQLUSION"; /*
	string pre_s1 = "BETWEENSUBTLESHADINGANDTHEABSENCEOFLIGHTLIESTHENUANCEOFIQLUSION";*/
	string pre_s2 = "ITWASTOTALLYINVISIBLEHOWSTHATPOSSIBLE?THEYUSEDTHEEARTHSMAGNETICFIELDXTHEINFORMATIONWASGATHEREDANDTRANSMITTEDUNDERGRUUNDTOANUNKNOWNLOCATIONXDOESLANGLEYKNOWABOUTTHIS?THEYSHOULDITSBURIEDOUTTHERESOMEWHEREXWHOKNOWSTHEEXACTLOCATION?ONLYWWTHISWASHISLASTMESSAGEXTHIRTYEIGHTDEGREESFIFTYSEVENMINUTESSIXPOINTFIVESECONDSNORTHSEVENTYSEVENDEGREESEIGHTMINUTESFORTYFOURSECONDSWESTIDBYROWS";/*
	string pre_s2 = "ITWASTOTALLYINVISIBLEHOWSTHATPOSSIBLE?THEYUSEDTHEEARTHSMAGNETICFIELDXTHEINFORMATIONWASGATHEREDANDTRANSMITTEDUNDERGRUUNDTOANUNKNOWNLOCATIONXDOESLANGLEYKNOWABOUTTHIS?THEYSHOULDITSBURIEDOUTTHERESOMEWHEREXWHOKNOWSTHEEXACTLOCATION?ONLYWWTHISWASHISLASTMESSAGEXTHIRTYEIGHTDEGREESFIFTYSEVENMINUTESSIXPOINTFIVESECONDSNORTHSEVENTYSEVENDEGREESEIGHTMINUTESFORTYFOURSECONDSWESTIDBYROWS";*/
	string pre_s3 = "SLOWLYDESPARATLYSLOWLYTHEREMAINSOFPASSAGEDEBRISTHATENCUMBEREDTHELOWERPARTOFTHEDOORWAYWASREMOVEDWITHTREMBLINGHANDSIMADEATINYBREACHINTHEUPPERLEFTHANDCORNERANDTHENWIDENINGTHEHOLEALITTLEIINSERTEDTHECANDLEANDPEEREDINTHEHOTAIRESCAPINGFROMTHECHAMBERCAUSEDTHEFLAMETOFLICKERBUTPRESENTLYDETAILSOFTHEROOMWITHINEMERGEDFROMTHEMISTXCANYOUSEEANYTHINGQ?";/*
	string pre_s3 = "SLOWLYDESPARATLYSLOWLYTHEREMAINSOFPASSAGEDEBRISTHATENCUMBEREDTHELOWERPARTOFTHEDOORWAYWASREMOVEDWITHTREMBLINGHANDSIMADEATINYBREACHINTHEUPPERLEFTHANDCORNERANDTHENWIDENINGTHEHOLEALITTLEIINSERTEDTHECANDLEANDPEEREDINTHEHOTAIRESCAPINGFROMTHECHAMBERCAUSEDTHEFLAMETOFLICKERBUTPRESENTLYDETAILSOFTHEROOMWITHINEMERGEDFROMTHEMISTXCANYOUSEEANYTHINGQ?";*/
	string pre_s4 = "OBKRUOXOGHULBSOLIFBBWEASTNORTHEASTOTWTQSJQSSEKZZWATJKLUDIAWINFBBERLINCLOCKWGDKZXTJCDIGKUHUAUEKCAR";/*
	string pre_s4 = "OBKRUOXOGHULBSOLIFBBWEASTNORTHEASTOTWTQSJQSSEKZZWATJKLUDIAWINFBBERLINCLOCKWGDKZXTJCDIGKUHUAUEKCAR";*/

	string pre_s5 = "XLAYERTWO";

	string pre_s21 = "";
	string pre_s321 = "";

	// entire 'crawlback' string from "krypto...quvwxz" to "zkrypt...nquvwx"

	string pre_kryptcrawl = "KRYPTOSABCDEFGHIJLMNQUVWXZRYPTOSABCDEFGHIJLMNQUVWXZKYPTOSABCDEFGHIJLMNQUVWXZKRPTOSABCDEFGHIJLMNQUVWXZKRYTOSABCDEFGHIJLMNQUVWXZKRYPOSABCDEFGHIJLMNQUVWXZKRYPTSABCDEFGHIJLMNQUVWXZKRYPTOABCDEFGHIJLMNQUVWXZKRYPTOSBCDEFGHIJLMNQUVWXZKRYPTOSACDEFGHIJLMNQUVWXZKRYPTOSABDEFGHIJLMNQUVWXZKRYPTOSABCEFGHIJLMNQUVWXZKRYPTOSABCDFGHIJLMNQUVWXZKRYPTOSABCDEGHIJLMNQUVWXZKRYPTOSABCDEFHIJLMNQUVWXZKRYPTOSABCDEFGIJLMNQUVWXZKRYPTOSABCDEFGHJLMNQUVWXZKRYPTOSABCDEFGHILMNQUVWXZKRYPTOSABCDEFGHIJMNQUVWXZKRYPTOSABCDEFGHIJLNQUVWXZKRYPTOSABCDEFGHIJLMQUVWXZKRYPTOSABCDEFGHIJLMNUVWXZKRYPTOSABCDEFGHIJLMNQVWXZKRYPTOSABCDEFGHIJLMNQUWXZKRYPTOSABCDEFGHIJLMNQUVXZKRYPTOSABCDEFGHIJLMNQUVWZKRYPTOSABCDEFGHIJLMNQUVWX";

	// full kryptos string only, repeated ~97 times

	string pre_kryfull = "KRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZ";

	// partial or full kryptos ciphertext in various combos

	string pre_k12 = "EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFDVFPJUDEEHZWETZYVGWHKKQETGFQJNCEGGWHKK?DQMCPFQZDQMMIAGPFXHQRLGTIMVMZJANQLVKQEDAGDVFRPJUNGEUNAQZGZLECGYUXUEENJTBJLBQCRTBJDFHRRYIZETKZEMVDUFKSJHKFWHKUWQLSZFTIHHDDDUVH?DWKBFUFPWNTDFIYCUQZEREEVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDXFLGGTEZ?FKZBSFDQVGOGIPUFXHHDRKFFHQNTGPUAECNUVPDJMQCLQUMUNEDFQELZZVRRGKFFVOEEXBDMVPNFQXEZLGREDNQFMPNZGLFLPMRJQYALMGNUVPDXVKPDQUMEBEDMHDAFMJGZNUPLGEWJLLAETG";
	string pre_k14 = "EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFDOBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
	string pre_k124 = "EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFDVFPJUDEEHZWETZYVGWHKKQETGFQJNCEGGWHKK?DQMCPFQZDQMMIAGPFXHQRLGTIMVMZJANQLVKQEDAGDVFRPJUNGEUNAQZGZLECGYUXUEENJTBJLBQCRTBJDFHRRYIZETKZEMVDUFKSJHKFWHKUWQLSZFTIHHDDDUVH?DWKBFUFPWNTDFIYCUQZEREEVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDXFLGGTEZ?FKZBSFDQVGOGIPUFXHHDRKFFHQNTGPUAECNUVPDJMQCLQUMUNEDFQELZZVRRGKFFVOEEXBDMVPNFQXEZLGREDNQFMPNZGLFLPMRJQYALMGNUVPDXVKPDQUMEBEDMHDAFMJGZNUPLGEWJLLAETGOBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
	string pre_k1234 = "EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFDVFPJUDEEHZWETZYVGWHKKQETGFQJNCEGGWHKK?DQMCPFQZDQMMIAGPFXHQRLGTIMVMZJANQLVKQEDAGDVFRPJUNGEUNAQZGZLECGYUXUEENJTBJLBQCRTBJDFHRRYIZETKZEMVDUFKSJHKFWHKUWQLSZFTIHHDDDUVH?DWKBFUFPWNTDFIYCUQZEREEVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDXFLGGTEZ?FKZBSFDQVGOGIPUFXHHDRKFFHQNTGPUAECNUVPDJMQCLQUMUNEDFQELZZVRRGKFFVOEEXBDMVPNFQXEZLGREDNQFMPNZGLFLPMRJQYALMGNUVPDXVKPDQUMEBEDMHDAFMJGZNUPLGEWJLLAETGENDYAHROHNLSRHEOCPTEOIBIDYSHNAIACHTNREYULDSLLSLLNOHSNOSMRWXMNETPRNGATIHNRARPESLNNELEBLPIIACAEWMTWNDITEENRAHCTENEUDRETNHAEOETFOLSEDTIWENHAEIOYTEYQHEENCTAYCREIFTBRSPAMHHEWENATAMATEGYEERLBTEEFOASFIOTUETUAEOTOARMAEERTNRTIBSEDDNIAAHTTMSTEWPIEROAGRIEWFEBAECTDDHILCEIHSITEGOEAOSDDRYDLORITRKLMLEHAGTDHARDPNEOHMGFMFEUHEECDMRIPFEIMEHNLSSTTRTVDOHW?OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
	string pre_k21 = "VFPJUDEEHZWETZYVGWHKKQETGFQJNCEGGWHKK?DQMCPFQZDQMMIAGPFXHQRLGTIMVMZJANQLVKQEDAGDVFRPJUNGEUNAQZGZLECGYUXUEENJTBJLBQCRTBJDFHRRYIZETKZEMVDUFKSJHKFWHKUWQLSZFTIHHDDDUVH?DWKBFUFPWNTDFIYCUQZEREEVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDXFLGGTEZ?FKZBSFDQVGOGIPUFXHHDRKFFHQNTGPUAECNUVPDJMQCLQUMUNEDFQELZZVRRGKFFVOEEXBDMVPNFQXEZLGREDNQFMPNZGLFLPMRJQYALMGNUVPDXVKPDQUMEBEDMHDAFMJGZNUPLGEWJLLAETGEMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFD";
	string pre_k24 = "VFPJUDEEHZWETZYVGWHKKQETGFQJNCEGGWHKK?DQMCPFQZDQMMIAGPFXHQRLGTIMVMZJANQLVKQEDAGDVFRPJUNGEUNAQZGZLECGYUXUEENJTBJLBQCRTBJDFHRRYIZETKZEMVDUFKSJHKFWHKUWQLSZFTIHHDDDUVH?DWKBFUFPWNTDFIYCUQZEREEVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDXFLGGTEZ?FKZBSFDQVGOGIPUFXHHDRKFFHQNTGPUAECNUVPDJMQCLQUMUNEDFQELZZVRRGKFFVOEEXBDMVPNFQXEZLGREDNQFMPNZGLFLPMRJQYALMGNUVPDXVKPDQUMEBEDMHDAFMJGZNUPLGEWJLLAETGOBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
	string pre_k321 = "";
	string pre_k32 = "";

	// presets - alphabets

	string preset_abc = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // technically useless

	string preset_kry = "KRYPTOSABCDEFGHIJLMNQUVWXZ"; // faster than 'kryptos'

	string preset_cia = "CYNTHIABDEFGJKLMOPQRSUVWXZ"; // secret?

	// custom alphas

	

	// ABCDEFGHIJKLMNOPQRSTUVWXYZ
	// ABCDEFGHIJKLMNOPQRSTUVWXYZ
	// 
	// WHISPERABCDFGJKLMNOQTUVXYZ
	//
	//
	// KRYPTOSABCDEFGHIJLMNQUVWXZ
	// KRYPTOSABCDEFGHIJLMNQUVWXZ
	// KRYPTOSABCDEFGHIJLMNQUVWXZ
	// KRYPTOSABCDEFGHIJLMNQUVWXZ
	// KRYPTOSABCDEFGHIJLMNQUVWXZ
	// KRYPTOSABCDEFGHIJLMNQUVWXZ
	// KRYPTOSABCDEFGHIJLMNQUVWXZ

	// kryptos crawlbacks - from -1 to -25 shift

	// string preset_kry = "KRYPTOSABCDEFGHIJLMNQUVWXZ";

	string preset_kry0 = "KRYPTOSABCDEFGHIJLMNQUVWXZ"; // just for sanity ocd
	string preset_kry1 = "RYPTOSABCDEFGHIJLMNQUVWXZK";
	string preset_kry2 = "YPTOSABCDEFGHIJLMNQUVWXZKR";
	string preset_kry3 = "PTOSABCDEFGHIJLMNQUVWXZKRY";
	string preset_kry4 = "TOSABCDEFGHIJLMNQUVWXZKRYP";
	string preset_kry5 = "OSABCDEFGHIJLMNQUVWXZKRYPT";
	string preset_kry6 = "SABCDEFGHIJLMNQUVWXZKRYPTO";
	string preset_kry7 = "ABCDEFGHIJLMNQUVWXZKRYPTOS";
	string preset_kry8 = "BCDEFGHIJLMNQUVWXZKRYPTOSA";
	string preset_kry9 = "CDEFGHIJLMNQUVWXZKRYPTOSAB";
	string preset_kry10 = "DEFGHIJLMNQUVWXZKRYPTOSABC";
	string preset_kry11 = "EFGHIJLMNQUVWXZKRYPTOSABCD";
	string preset_kry12 = "FGHIJLMNQUVWXZKRYPTOSABCDE";
	string preset_kry13 = "GHIJLMNQUVWXZKRYPTOSABCDEF";
	string preset_kry14 = "HIJLMNQUVWXZKRYPTOSABCDEFG";
	string preset_kry15 = "IJLMNQUVWXZKRYPTOSABCDEFGH";
	string preset_kry16 = "JLMNQUVWXZKRYPTOSABCDEFGHI";
	string preset_kry17 = "LMNQUVWXZKRYPTOSABCDEFGHIJ";
	string preset_kry18 = "MNQUVWXZKRYPTOSABCDEFGHIJL";
	string preset_kry19 = "NQUVWXZKRYPTOSABCDEFGHIJLM";
	string preset_kry20 = "QUVWXZKRYPTOSABCDEFGHIJLMN";
	string preset_kry21 = "UVWXZKRYPTOSABCDEFGHIJLMNQ";
	string preset_kry22 = "VWXZKRYPTOSABCDEFGHIJLMNQU";
	string preset_kry23 = "WXZKRYPTOSABCDEFGHIJLMNQUV";
	string preset_kry24 = "XZKRYPTOSABCDEFGHIJLMNQUVW";
	string preset_kry25 = "ZKRYPTOSABCDEFGHIJLMNQUVWX";


	// long alphas

	string preset_kryptos = "KRYPTOSABCDEFGHIJLMNQUVWXZRYPTOSABCDEFGHIJLMNQUVWXZKYPTOSABCDEFGHIJLMNQUVWXZKRPTOSABCDEFGHIJLMNQUVWXZKRYTOSABCDEFGHIJLMNQUVWXZKRYPOSABCDEFGHIJLMNQUVWXZKRYPTSABCDEFGHIJLMNQUVWXZKRYPTOABCDEFGHIJLMNQUVWXZKRYPTOSBCDEFGHIJLMNQUVWXZKRYPTOSACDEFGHIJLMNQUVWXZKRYPTOSABDEFGHIJLMNQUVWXZKRYPTOSABCEFGHIJLMNQUVWXZKRYPTOSABCDFGHIJLMNQUVWXZKRYPTOSABCDEGHIJLMNQUVWXZKRYPTOSABCDEFHIJLMNQUVWXZKRYPTOSABCDEFGIJLMNQUVWXZKRYPTOSABCDEFGHJLMNQUVWXZKRYPTOSABCDEFGHILMNQUVWXZKRYPTOSABCDEFGHIJMNQUVWXZKRYPTOSABCDEFGHIJLNQUVWXZKRYPTOSABCDEFGHIJLMQUVWXZKRYPTOSABCDEFGHIJLMNUVWXZKRYPTOSABCDEFGHIJLMNQVWXZKRYPTOSABCDEFGHIJLMNQUWXZKRYPTOSABCDEFGHIJLMNQUVXZKRYPTOSABCDEFGHIJLMNQUVWZKRYPTOSABCDEFGHIJLMNQUVWX";
	string preset_kryfull = "KRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZKRYPTOSABCDEFGHIJLMNQUVWXZ";
	string preset_apk = "KARBYCPDTEOFSGAHBICJDKELFMGNHOIPJQLRMSNTQUUVVWWXXYZZ";
	string preset_john = "INTHEBEGINNINGWASTHEWORDANDTHEWORDWASMADEFLESHCJKPQUVXYZ";

	/* ABOVE ARE ALPHABET STRINGS AND PRESET CIPHERTEXTS USED TO SAVE TIME - THEY COME IN HANDY */
	/* BELOW IS MESSY BUT SIMPLE AND REDUNDANT - FORGIVE ME I AM MENTALLY HANDICRAPT */

	// sets the starting lines, after defaults loaded ofc

	call_title();
	call_funcs();

	while (true) {
		/*SHELL*/
		cout << "   >> ";
		string input;
		getline(cin, input);

		/* COMMAND INPUTS */
		if (input.substr(0, 1) == "~") {
			size_t space = input.find(' ', 1);
			string subCommand, value;
			if (space != string::npos) {
				subCommand = input.substr(1, space - 1);
				value = input.substr(space + 1);
			}
			else {
				subCommand = input.substr(1);
				value = "";
			}

			/*if (subCommand == "#") {
				if (!value.empty()) {
					hash = value;
				}
				else {
					cout << "\t[Error]\n \t\tNo value | CIPHERTEXT NOT SET!\n";
				}
			}
			else if (subCommand == "!") {
				if (!value.empty()) {
					cipher.setKey(value);
				}
				else {
					cout << "\t[Error]\n \t\tNo value | KEY NOT SET!\n";
				}
			}
			else if (subCommand == "@") {
				if (!value.empty()) {
					cipher.setAlpha(value);
				}
				else {
					cout << "\t[Error]\n \t\tNo value | ALPHABET NOT SET!\n";
				}
			}*/

			if (subCommand == "g") {
				cout << "\tGood morning!\n";
			}
			else if (subCommand == "r") {
				rev_Str(hash);
			}
			else if (subCommand == "0") {
				cout << "\tConverted [Modulus+0] = " << hash << "\n";
			}
			else if (subCommand == "1") {
				cout << "\tConverted [Modulus+1] = " << hash << "\n";
			}
			else if (subCommand == "") {
				cout << "\t[Error]\n \t\tNo syntax | <c/w/a> <string>\n";
			}
		}

		/* CIPHERING INPUTS*/

		if (input.substr(0, 1) == "!") {
			size_t space = input.find(' ', 1);
			string subCommand, value;

			if (space != string::npos) {
				subCommand = input.substr(1, space - 1);
				value = input.substr(space + 1);
			}
			else {
				subCommand = input.substr(1);
				value = "";
			}
			if (!value.empty()) {
				cipher.setKey(value);
			}
		}

		if (input.substr(0, 1) == "@") {
			size_t space = input.find(' ', 1);
			string subCommand, value;

			if (space != string::npos) {
				subCommand = input.substr(1, space - 1);
				value = input.substr(space + 1);
			}
			else {
				subCommand = input.substr(1);
				value = "";
			}
			if (!value.empty()) {
				cipher.setAlpha(value);
			}
		}

		if (input.substr(0, 1) == "#") {
			size_t space = input.find(' ', 1);
			string subCommand, value;

			if (space != string::npos) {
				subCommand = input.substr(1, space - 1);
				value = input.substr(space + 1);
			}
			else {
				subCommand = input.substr(1);
				value = "";
			}
			if (!value.empty()) {
				hash = value;
			}
		}

		/*ENCRYPTION OUTPUTS*/

		if (input == "e" || input == "d" || input == "q" || input == "z") {
			string encrypted = cipher.e_c(hash);
			string decrypted = cipher.d_c(hash);

			if (input == "e") {
				cout << "[Enc] # " << encrypted << "\n";
			}
			else if (input == "d") {
				cout << "[Dec] $ " << decrypted << "\n";
			}
			// do all!
			else if (input == "q") {
				cout << "[Wrd] @ " << fetchKey << "\n";
				cout << "[Abc] @ " << cipher.alpha << "\n";
				cout << "[Key] ! " << cipher.key << "\n";
				cout << "[Cph] # " << hash << "\n";
				cout << "[Enc] # " << encrypted << "\n";
				cout << "[Dec] $ " << decrypted << "\n";
			}
			else if (input == "z") {
				cout << "[Enc] # " << encrypted << "\n";
				cout << "[Dec] $ " << decrypted << "\n";
			}

			saveStateToFile(hash, cipher.key, fetchKey, cipher.alpha, encrypted, decrypted);
		}

		// STATIC VARIABLES CHECK ON KEYS

		else if (input == "xc") {
			cout << "[Abc] @ " << cipher.alpha << "\n";
			cout << "[Key] ! " << cipher.key << "\n";
			cout << "[Cph] # " << hash << "\n";
			
		}
		else if (input == "w") {
			cout << "[Key] ! " << cipher.key << "\n";
		}
		else if (input == "a") {
			cout << "[Abc] @ " << cipher.alpha << "\n";
		}
		else if (input == "c") {
			cout << "[Cph] # " << hash << "\n";
		}
		else if (input == "s") {
			cout << "[Wrd] @ " << fetchKey << "\n";
		}

		/*PERIOD COMMAND ".<string>" */
		/*USED BY INPUTTING A "USE KEY" TO FORM A SINGLETON KEYED ALPHABET*/

		if (input.substr(0, 1) == ".") {
			size_t space = input.find(' ', 1);
			string subCommand, value;

			if (space != string::npos) {
				subCommand = input.substr(1, space - 1);
				value = input.substr(space + 1);
			}
			else {
				subCommand = input.substr(1);
				value = "";
			}

			string parsKey = value.empty() ? subCommand : value;
			fetchKey = parsKey;

			string keyAlpha = setKeyAlpha(parsKey);

			cout << "[Abc] @ " << keyAlpha << "\n";
			cipher.alpha = keyAlpha;
		}

		/*COLON COMMAND ":<string>" */
		/*USED FOR PRESET VALUES ONLY*/

		if (input.substr(0, 1) == ":") {
			size_t space = input.find(' ', 1);
			string subCommand, value;

			if (space != string::npos) {
				subCommand = input.substr(1, space - 1);
				value = input.substr(space + 1);
			}
			else {
				subCommand = input.substr(1);
				value = "";
			}

			if (subCommand == "abc") {
				cipher.setAlpha(preset_abc);
				cout << "[Abc] @ " << preset_abc << "\n";
			}
			else if (subCommand == "kry") {
				cipher.setAlpha(preset_kry);
				cout << "[Abc] @ " << preset_kry << "\n";
			}
			else if (subCommand == "cia") {
				cipher.setAlpha(preset_cia);
				cout << "[Abc] @ " << preset_cia << "\n";
			}

			// preset ciphertexts



			if (subCommand == "k0") {
				hash = pre_k0;
				cout << "[Cph] # " << pre_k0 << "\n";
			}
			else if (subCommand == "k1") {
				hash = pre_k1;
				cout << "[Cph] # " << pre_k1 << "\n";
			}
			else if (subCommand == "k2") {
				hash = pre_k2;
				cout << "[Cph] # " << pre_k2 << "\n";
			}
			else if (subCommand == "k3") {
				hash = pre_k3;
				cout << "[Cph] # " << pre_k3 << "\n";
			}
			else if (subCommand == "k4") {
				hash = pre_k4;
				cout << "[Cph] # " << pre_k4 << "\n";
			}
			else if (subCommand == "k5") {
				hash = pre_k5;
				cout << "[Cph] # " << pre_k5 << "\n";
			}
			else if (subCommand == "kryptcrawl") {
				hash = pre_kryptcrawl;
				cout << "[Cph] # " << pre_kryptcrawl << "\n";
			}
			else if (subCommand == "kryfull") {
				hash = pre_kryfull;
				cout << "[Cph] # " << pre_kryfull << "\n";
			}
			else if (subCommand == "k12") {
				hash = pre_k12;
				cout << "[Cph] # " << pre_k12 << "\n";
			}
			else if (subCommand == "k14") {
				hash = pre_k14;
				cout << "[Cph] # " << pre_k14 << "\n";
			}
			else if (subCommand == "k124") {
				hash = pre_k124;
				cout << "[Cph] # " << pre_k124 << "\n";
			}
			else if (subCommand == "k1234") {
				hash = pre_k1234;
				cout << "[Cph] # " << pre_k1234 << "\n";
			}
			else if (subCommand == "k21") {
				hash = pre_k21;
				cout << "[Cph] # " << pre_k21 << "\n";
			}
			else if (subCommand == "k24") {
				hash = pre_k24;
				cout << "[Cph] # " << pre_k24 << "\n";
			}
			else if (subCommand == "k321") {
				hash = pre_k321;
				cout << "[Cph] # " << pre_k321 << "\n";
			}
			else if (subCommand == "k32") {
				hash = pre_k32;
				cout << "[Cph] # " << pre_k32 << "\n";
			}


			// solution presets 

			if (subCommand == "s0") {
				hash = pre_s0;
				cout << "[Cph] # " << pre_s0 << "\n";
			}
			else if (subCommand == "s1") {
				hash = pre_s1;
				cout << "[Cph] # " << pre_s1 << "\n";
			}
			else if (subCommand == "s2") {
				hash = pre_s2;
				cout << "[Cph] # " << pre_s2 << "\n";
			}
			else if (subCommand == "s3") {
				hash = pre_s3;
				cout << "[Cph] # " << pre_s3 << "\n";
			}
			else if (subCommand == "s4") {
				hash = pre_s4;
				cout << "[Cph] # " << pre_s4 << "\n";
			}
			else if (subCommand == "s5") {
				hash = pre_s5;
				cout << "[Cph] # " << pre_s5 << "\n";
			}

			// krytpos crawlbacks

			else if (subCommand == "kry-0") {
				cipher.setAlpha(preset_kry0);
				cout << "[Abc] @ " << preset_kry0 << "\n";
			}
			else if (subCommand == "kry-1") {
				cipher.setAlpha(preset_kry1);
				cout << "[Abc] @ " << preset_kry1 << "\n";
			}
			else if (subCommand == "kry-2") {
				cipher.setAlpha(preset_kry2);
				cout << "[Abc] @ " << preset_kry2 << "\n";
			}
			else if (subCommand == "kry-3") {
				cipher.setAlpha(preset_kry3);
				cout << "[Abc] @ " << preset_kry3 << "\n";
			}
			else if (subCommand == "kry-4") {
				cipher.setAlpha(preset_kry4);
				cout << "[Abc] @ " << preset_kry4 << "\n";
			}
			else if (subCommand == "kry-5") {
				cipher.setAlpha(preset_kry5);
				cout << "[Abc] @ " << preset_kry5 << "\n";
			}
			else if (subCommand == "kry-6") {
				cipher.setAlpha(preset_kry6);
				cout << "[Abc] @ " << preset_kry6 << "\n";
			}
			else if (subCommand == "kry-7") {
				cipher.setAlpha(preset_kry7);
				cout << "[Abc] @ " << preset_kry7 << "\n";
			}
			else if (subCommand == "kry-8") {
				cipher.setAlpha(preset_kry8);
				cout << "[Abc] @ " << preset_kry8 << "\n";
			}
			else if (subCommand == "kry-9") {
				cipher.setAlpha(preset_kry9);
				cout << "[Abc] @ " << preset_kry9 << "\n";
			}
			else if (subCommand == "kry-10") {
				cipher.setAlpha(preset_kry10);
				cout << "[Abc] @ " << preset_kry10 << "\n";
			}
			else if (subCommand == "kry-11") {
				cipher.setAlpha(preset_kry11);
				cout << "[Abc] @ " << preset_kry11 << "\n";
			}
			else if (subCommand == "kry-12") {
				cipher.setAlpha(preset_kry12);
				cout << "[Abc] @ " << preset_kry12 << "\n";
			}
			else if (subCommand == "kry-13") {
				cipher.setAlpha(preset_kry13);
				cout << "[Abc] @ " << preset_kry13 << "\n";
			}
			else if (subCommand == "kry-14") {
				cipher.setAlpha(preset_kry14);
				cout << "[Abc] @ " << preset_kry14 << "\n";
			}
			else if (subCommand == "kry-15") {
				cipher.setAlpha(preset_kry15);
				cout << "[Abc] @ " << preset_kry15 << "\n";
			}
			else if (subCommand == "kry-16") {
				cipher.setAlpha(preset_kry16);
				cout << "[Abc] @ " << preset_kry16 << "\n";
			}
			else if (subCommand == "kry-17") {
				cipher.setAlpha(preset_kry17);
				cout << "[Abc] @ " << preset_kry17 << "\n";
			}
			else if (subCommand == "kry-18") {
				cipher.setAlpha(preset_kry18);
				cout << "[Abc] @ " << preset_kry18 << "\n";
			}
			else if (subCommand == "kry-19") {
				cipher.setAlpha(preset_kry19);
				cout << "[Abc] @ " << preset_kry19 << "\n";
			}
			else if (subCommand == "kry-20") {
				cipher.setAlpha(preset_kry20);
				cout << "[Abc] @ " << preset_kry20 << "\n";
			}
			else if (subCommand == "kry-21") {
				cipher.setAlpha(preset_kry21);
				cout << "[Abc] @ " << preset_kry21 << "\n";
			}
			else if (subCommand == "kry-22") {
				cipher.setAlpha(preset_kry22);
				cout << "[Abc] @ " << preset_kry22 << "\n";
			}
			else if (subCommand == "kry-23") {
				cipher.setAlpha(preset_kry23);
				cout << "[Abc] @ " << preset_kry23 << "\n";
			}
			else if (subCommand == "kry-24") {
				cipher.setAlpha(preset_kry24);
				cout << "[Abc] @ " << preset_kry24 << "\n";
			}
			else if (subCommand == "kry-25") {
				cipher.setAlpha(preset_kry25);
				cout << "[Abc] @ " << preset_kry25 << "\n";
			}
			else if (subCommand == "kry") {
				cipher.setAlpha(preset_kry);
				cout << "[Abc] @ " << preset_kry << "\n";
			}
			else if (subCommand == "kryfull") {
				cipher.setAlpha(preset_kryfull);
				cout << "[Abc] @ " << preset_kryfull << "\n";
			}
			else if (subCommand == "apk") {
				cipher.setAlpha(preset_apk);
				cout << "[Abc] @ " << preset_apk << "\n";
			}
			else if (subCommand == "john") {
				cipher.setAlpha(preset_john);
				cout << "[Abc] @ " << preset_john << "\n";
			}
		}

		if (input.substr(0, 1) == "^") {
			size_t space = input.find(' ', 1);
			string subCommand, value;

			if (space != string::npos) {
				subCommand = input.substr(1, space - 1);
				value = input.substr(space + 1);
			}
			else {
				subCommand = input.substr(1);
				value = "";
			}

			if (subCommand == "c") {
				rev_Str(hash);
				cout << "[Cph] # " << hash << "\n";
			}
			else if (subCommand == "w") {
				rev_Str(cipher.key);
				cout << "[Key] ! " << cipher.key << "\n";
			}
			else if (subCommand == "a") {
				rev_Str(cipher.alpha);
				cout << "[Abc] @ " << cipher.alpha << "\n";
			}
		}

		if (input.substr(0, 1) == "*") {
			size_t space = input.find(' ', 1);
			string subCommand, value;

			if (space != string::npos) {
				subCommand = input.substr(1, space - 1);
				value = input.substr(space + 1);
			}
			else {
				subCommand = input.substr(1);
				value = "";
			}
			if (subCommand == "a") {
				gen_tabl(cipher.alpha);
				cout << "value?: \n";
			}
			if (subCommand == "1") {
				cout << "[Mod] = " << list_mod1(hash) << "\n";
			}
			if (subCommand == "0") {
				cout << "[Mod] = " << list_mod0(hash) << "\n";
			}
		}

		/* Other commands */

		else if (input == "~p") {
			saveStateToFile(hash, cipher.key, fetchKey, cipher.alpha, cipher.e_c(hash), cipher.d_c(hash));
			call_print();
		}
		/*else if (input == "~poff") {
			doesnotprint();
		}
		else if (input == "~pon") {
			savesprinting();
		}*/
		else if (input == "~cls") {
			clearScreen();

		}
		else if (input == "~reset") {
			clearScreen();
			call_title();
			call_funcs();
		}
		else if (input == "~help") {
			call_manual();
		}
		else if (input == "~ver") {
			call_version();
		}
		else if (input == "~def") {
			call_defaults();
		}
		else if (input == "~q!") {
			call_quit();
			break;
		}
	}
	return 0;
};