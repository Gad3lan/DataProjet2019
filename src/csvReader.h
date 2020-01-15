#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

/**
 * Lit le CSV et le mets dans une string.
 * 
 * @param name Chemin vers le CSV
 * @return string String contenant le CSV
 */
string readCSVFile(string name);
/**
 * Charge la string dans des tableaux
 * 
 * @param csv String contenant le csv
 * @param teams Nom des equipes
 * @param scores Score des equipes par match
 * @param ranks Rang des equipes par match
 */
void parse(string csv, string teams[20], int scores[20][38], int ranks[20][38]);
/**
 * Affichage du tableau des scores
 * 
 * @param scores Tableau des scores
 */
void aff(int scores[20][38]);

#endif