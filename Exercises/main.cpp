///
/// Ce projet utilise quelques librairies disponibles sur Vcpkg et permet entre autre d'en tester le bon fonctionnement.
///


#include <cstddef>
#include <cstdint>

#include <array>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <span>
#include <utility>

#include <cppitertools/itertools.hpp>
#include <cppitertools/zip.hpp>

#include "MyClass.hpp"
#include "School.hpp"
#include "Clock.hpp"

using namespace std;
using namespace iter;


void runClockExample() {
	Clock c1(true);
	TravelClock c2(true, "Final Stage", +6);
	TravelClock c3(false, "Hyrule", -10);
	cout << c1 << "\n" << c2 << "\n" << c3 << "\n\n";

	vector<Clock*> clocks = {&c1, &c2, &c3};
	for (auto&& c : clocks)
		cout << *c << "\n";
	cout << "\n";
}

void runInheritanceExample() {
	/*
	    MyClass (dtor virtuel)
	       ^
	       |
	  MyNiceClass

	  MyOtherClass (dtor r�gulier)
	       ^
	       |
	MyOtherNiceClass
	*/

	// On peut cr�er des unique_ptr d'une classe d�riv�e et de les mettre dans un unique_ptr d'une classe de base, car les unique_ptr supporte le upcasting.
	unique_ptr<MyClass> anickClermont = make_unique<MyNiceClass>(42);
	cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
	unique_ptr<MyOtherClass> mathieuSavoie = make_unique<MyOtherNiceClass>(0xBADF00D);
	cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;

	// Si on d�truit anickClermont, on voit que le destructeur se fait appeler correctement, car il est virtuel.
	anickClermont.reset();
	cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
	// Toutefois, pour mathieuSavoie, le destructeur n'est pas virtuel. Or, unique_ptr<MyOtherClass> ne sait pas qu'il faut appeler le destructeur de MyOtherNiceClass, et donc appelle seulement celui de MyOtherClass.
	mathieuSavoie.reset();
	cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
	// Si on avait directement un unique_ptr (ou un objet sur la pile) de MyOtherNiceClass, on n'a pas ce probl�me. C'est donc dans le cas o� le destructeur est appel� � partir d'un pointeur/r�f�rence up-cast�, et donc devrait �tre appel� virtuellement.
	auto maxenceMathieu = make_unique<MyOtherNiceClass>();
	cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
	maxenceMathieu.reset();
}

void runConversionExample() {
	// Op�rateurs de conversion :
	//  - static_cast
	//  - dynamic_cast
	//  - const_cast
	//  - reinterpret_cast
	//  - C-style cast : (int)foo
	{
		double pierre = 42.42;
		// Conversion de r�el � entier, donc troncation.
		int vicky = static_cast<int>(pierre);
		MyNiceClass mathias;
		// Faire un upcasting est un static_cast, mais peut �tre implicite.
		MyClass& kim = static_cast<MyClass&>(mathias);
		// On peut faire un downcast avec static_cast, mais aucune v�rification n'est faite.
		MyNiceClass& charlotte = static_cast<MyNiceClass&>(kim);
		// � l'ex�cution, dynamic_cast v�rifie que le downcast est valide. Si la v�rification �choue pour une r�f�rence, une exception est lanc� (le programme plante).
		MyNiceClass& julia = dynamic_cast<MyNiceClass&>(kim);
		// Si le dynamic_cast pour un pointeur �choue, un pointeur nul est retourn�.
		MyNiceClass* cassandra = dynamic_cast<MyNiceClass*>(&kim);
	}
	{
		MyNiceClass lydia;
		// Prendre une r�f�rence constante vers un objet non-constant ajoute une contrainte et est implicite (donc fait un const_cast implicitement). Le upcasting fait un static_cast implicitement.
		const MyClass& lydiaConst = lydia;
		// Pour enlever la constance, il faut faire const_cast, pour upcaster, il faut faire dynamic_cast (ou static_cast).
		MyNiceClass& lydiaOrig = const_cast<MyNiceClass&>(dynamic_cast<const MyNiceClass&>(lydiaConst));
	}
	{
		double claudia = 42.42;
		double* claudiaPtr = &claudia;
		// reinterpret_cast ne change rien aux donn�es. 'louis' va contenir la m�me adresse que 'claudiaPtr', mais en pensant que c'est un int � 8 octets (je compile en 64 bit).
		uint64_t* louis = reinterpret_cast<uint64_t*>(claudiaPtr);
		// Dans ce cas, le c-style cast fait un static_cast (donc troncation).
		uint64_t ethan = (uint64_t)claudia;
		// Dans ce cas, le c-style cast fait un reinterpret_cast, 'yoan' va contenir sous forme d'entier l'adresse point�e par 'claudiaPtr'. Les bits sont inchang�s.
		uint64_t yoan = (uint64_t)claudiaPtr;
	}
}


int main() {
	runClockExample();
	cout << "\n\n\n\n";
	runInheritanceExample();
	cout << "\n\n\n\n";
	runConversionExample();
}

