#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
	
	fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
	fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
	fMessenger->DeclareProperty("isCherenkov", isCherenkov, "Toggle Cherenkov setup");
	fMessenger->DeclareProperty("isScintillator", isScintillator, "Toggle Scintillator setup");
	fMessenger->DeclareProperty("isGeDetector", isGeDetector, "Toggle GeDetector setup");
	
	nCols = 1;
	nRows = 1;
	
	DefineMaterials();
	
	xWorld = 1.1*m;
	yWorld = 1.1*m;
	zWorld = 1.5*m;
	
	isCherenkov = false;
	isScintillator = false;
	isGeDetector = true;
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
	G4NistManager *nist = G4NistManager::Instance();
	
	G4double energy[2] = {1*eV, 8*eV};
	G4double rindexAerogel[2] = {1.1, 1.1};
	G4double rindexWorld[2] = {1.0, 1.0};
	G4double rindexNaI[2] = {1.59, 1.59};
	G4double rindexSi[2] = {1.5, 1.5};
	G4double reflectivity[2] = {1.0, 1.0};
	G4double abslength2[2]= {100*m,100*m}; 
	G4double abslength3[2]= {1*nm,1*nm};
	
	SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
	SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
	SiO2->AddElement(nist->FindOrBuildElement("O"), 2);
	
	G4MaterialPropertiesTable *mptSiO2 = new G4MaterialPropertiesTable();
	mptSiO2->AddProperty("ABSLENGTH", energy, abslength3, 2);
	mptSiO2->AddProperty("RINDEX", energy, rindexSi, 2);
	SiO2->SetMaterialPropertiesTable(mptSiO2);
	
	H2O = new G4Material("H2O", 1.000*g/cm3, 2);
	H2O->AddElement(nist->FindOrBuildElement("H"), 2);
	H2O->AddElement(nist->FindOrBuildElement("O"), 1);
	
	C = nist->FindOrBuildElement("C");
	
	Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
	Aerogel->AddMaterial(SiO2, 62.5*perCent);
	Aerogel->AddMaterial(H2O, 37.4*perCent);
	Aerogel->AddElement(C, 00.1*perCent);
	
	worldMat = nist->FindOrBuildMaterial("G4_AIR");
	
        water = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
	
	
	G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
	mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);
	
	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
	mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
	mptWorld->AddProperty("ABSLENGTH", energy, abslength2, 2);
        
	
	Aerogel->SetMaterialPropertiesTable(mptAerogel);
	
	worldMat->SetMaterialPropertiesTable(mptWorld);
	
	Na = nist->FindOrBuildElement("Na");
	I = nist->FindOrBuildElement("I");
	NaI = new G4Material("NaI", 3.87*g/cm3, 2);
	NaI->AddElement(Na, 1);
	NaI->AddElement(I, 1);
	
	G4double fraction[2]= {1.0,1.0}; 
	G4double fraction1[2]= {0,0}; 
	G4double fraction2[2]= {0,0}; 
	G4double abslength[2]= {3*m,3*m}; 
	
	G4MaterialPropertiesTable *mptH2O = new G4MaterialPropertiesTable();
		water->SetMaterialPropertiesTable(mptH2O);
	mptH2O->AddProperty("RINDEX", energy, rindexNaI, 2);
	mptH2O->AddProperty("ABSLENGTH", energy, abslength, 2);
	mptH2O->AddProperty("SCINTILLATIONCOMPONENT1", energy, fraction, 2);
	//mptH2O->AddProperty("SCINTILLATIONCOMPONENT2", energy, fraction1, 2);
	//mptH2O->AddProperty("SCINTILLATIONCOMPONENT3", energy, fraction2, 2);
	mptH2O->AddConstProperty("SCINTILLATIONYIELD", 5000.0/MeV);
	//mptH2O->AddConstProperty("SCINTILLATIONYIELD", 0./MeV);
	//mptH2O->AddConstProperty("SCINTILLATIONYIELD", 0./MeV);
	mptH2O->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 250*ns);
	//mptH2O->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 600*ns);
	//mptH2O->AddConstProperty("SCINTILLATIONTIMECONSTANT3", 800*ns);
	mptH2O->AddConstProperty("RESOLUTIONSCALE", 1.);
	mptH2O->AddConstProperty("SCINTILLATIONYIELD1", 1.);
	//mptH2O->AddConstProperty("SCINTILLATIONYIELD2", 0.);
	//mptH2O->AddConstProperty("SCINTILLATIONYIELD3", 0.);
	water->SetMaterialPropertiesTable(mptH2O);
	water->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
	
	mirror = new G4OpticalSurface("mirror");
	mirror->SetType(dielectric_metal);
	mirror->SetFinish(ground);
	mirror->SetModel(unified);
	G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
	mptMirror->AddProperty("REFLECTIVITY", energy, reflectivity, 2);
	mirror->SetMaterialPropertiesTable(mptMirror);





	// Новый материал

	Al = nist->FindOrBuildElement("Al");
	Alu = new G4Material("Al", 2.7*g/cm3, 1);
	Alu->AddElement(Al, 1);

	Ge = nist->FindOrBuildElement("Ge");
	Ger = new G4Material("Ge", 5.327*g/cm3, 1);
	Ger->AddElement(Ge, 1);

	Cu = nist->FindOrBuildElement("Cu");
	Cup = new G4Material("Cu", 8.89*g/cm3, 1);
	Cup->AddElement(Cu, 1);

	Pb = nist->FindOrBuildElement("Pb");
	Plu = new G4Material("Pb", 11.35*g/cm3, 1);
	Plu->AddElement(Pb, 1);

	// Материал источника

	K2CO3 = new G4Material("H2O", 2.43*g/cm3, 3);
	K2CO3->AddElement(nist->FindOrBuildElement("K"), 2);
	K2CO3->AddElement(nist->FindOrBuildElement("C"), 1);
	K2CO3->AddElement(nist->FindOrBuildElement("O"), 3);

}

void MyDetectorConstruction::ConstructCherenkov()
{	
G4double index=0;
G4double index2=5;
solidDetector = new G4Tubs("solidDetector", 0.*m, (0.025+0.005*index)*m, 0.01*m, 0*deg, 360*deg);
	
	logicDetector = new G4LogicalVolume(solidDetector, SiO2,"logicDetector");
	
	//for(G4int i = 0; i<nRows; i++)
	//{780
	//	for(G4int j = 0; j<nCols; j++)
	//	{
			physDetector = new G4PVPlacement(0,G4ThreeVector(0.*m,-0.*m, (0.11+0.05*index2)*m),logicDetector, "physDetector", logicWorld, false, 0, true);
	//	}
	//}
	
	solidScintillator = new G4Box("solidScintillator", (0.025+0.005*index)*m, (0.025+0.005*index)*m, (0.1+0.05*index2)*m);
	logicScintillator = new G4LogicalVolume(solidScintillator, water, "logicScintillator");
	for(G4double ii=0; ii<1; ii++)   //(от -1 до 2) 3*3 сцинтиллятора
	{
		for(G4double jj=0; jj<1; jj++) //(от -1 до 2) 3*3 сцинтиллятора
		{	
	physScintillator = new G4PVPlacement(0, G4ThreeVector(0.053*ii*m,0.053*jj*m,0.), logicScintillator, "physScintillator", logicWorld, false, 0, true);/*+0.005*/
		}
	}
	//physScintillator = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicScintillator, "physScintillator", logicWorld, false, 0, true);
}

void MyDetectorConstruction::ConstructScintillator()
{
	solidScintillator = new G4Box("solidScintillator", (0.025)*m, (0.025)*m, 0.035*m);
	logicScintillator = new G4LogicalVolume(solidScintillator, water, "logicScintillator");
	//G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirror);
	//fScoringVolume =logicScintillator;
	physScintillator = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicScintillator, "physScintillator", logicWorld, false, 0, true);
}

// Новый детектор

void MyDetectorConstruction::GeDetector()
{
	// Пассивная защита

	solidProtector1 = new G4Box("solidProtector1", (200.0)*mm, (285.0)*mm, (200.0)*mm);
	//logicProtector1 = new G4LogicalVolume(solidProtector1, water, "logicProtector1");
	//G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirror);
	//fScoringVolume =logicScintillator;
	//physProtector1 = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicProtector1, "physProtector1", logicWorld, false, 0, true);

	solidProtector2 = new G4Box("solidProtector2", (100.0)*mm, (225.0)*mm, (100.0)*mm);
	//logicProtector2 = new G4LogicalVolume(solidProtector2, water, "logicProtector2");
	//G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirror);
	//fScoringVolume =logicScintillator;
	//physProtector2 = new G4PVPlacement(0, G4ThreeVector(0.,60.,0.), logicProtector2, "physProtector2", logicWorld, false, 0, true);

	Subtract1 = new G4SubtractionSolid("Protector1-Protector2", solidProtector1, solidProtector2, 0, G4ThreeVector(0.,60.,0.));
	logicSubtract1 = new G4LogicalVolume(Subtract1, Plu, "logicSubtract1");
	//logicSubtract1->SetVisAttributes(G4Color::Blue());
	//G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirror);
	//fScoringVolume =logicScintillator;
	physSubtract1 = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicSubtract1, "physSubtract1", logicWorld, false, 0, true);

	solidProtector3 = new G4Box("solidProtector3", (50.0)*mm, (200.0)*mm, (50.0)*mm);
	//logicProtector2 = new G4LogicalVolume(solidProtector2, water, "logicProtector2");
	//G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirror);
	//fScoringVolume =logicScintillator;
	//physProtector2 = new G4PVPlacement(0, G4ThreeVector(0.,60.,0.), logicProtector2, "physProtector2", logicWorld, false, 0, true);

	Subtract2 = new G4SubtractionSolid("Protector2-Protector3", solidProtector2, solidProtector3, 0, G4ThreeVector(0.,60.,0.));
	logicSubtract2 = new G4LogicalVolume(Subtract2, Cup, "logicSubtract2");
	//G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirror);
	//fScoringVolume =logicScintillator;
	physSubtract2 = new G4PVPlacement(0, G4ThreeVector(0.,60.,0.), logicSubtract2, "physSubtract2", logicWorld, false, 0, true);

	// Алюминиевый корпус

	rotDet = new G4RotationMatrix();
	rotDet->rotateX(90.*deg);

	solidBody1 = new G4Tubs("solidBody1", 0.*mm, 41.5*mm, 30.65*mm, 0*deg, 360*deg);
	solidBody2 = new G4Tubs("solidBody2", 0.*mm, 40.5*mm, 29.65*mm, 0*deg, 360*deg);

	Body = new G4SubtractionSolid("solidBody1-solidBody2", solidBody1, solidBody2, 0, G4ThreeVector(0.,0.,1.));
	logicBody = new G4LogicalVolume(Body, Alu,"logicBody");
	physBody = new G4PVPlacement(rotDet,G4ThreeVector(0.,20.65, 0.),logicBody, "physBody", logicWorld, false, 0, true);

	// Детектор

	solidGeDetector = new G4Tubs("solidGeDetector", 0.*mm, 33.15*mm, 25.65*mm, 0*deg, 360*deg);
	logicGeDetector = new G4LogicalVolume(solidGeDetector, Ger,"logicGeDetector");
	physGeDetector = new G4PVPlacement(rotDet,G4ThreeVector(0.,20.65, 0.),logicGeDetector, "physGeDetector", logicWorld, false, 0, true);

	// Источник

	Source = new G4Cons("Source", 0.*mm, 42.5*mm, 0.*mm, 37.5*mm, 35.0*mm, 0*deg, 360*deg);
	logicSource = new G4LogicalVolume(Source, K2CO3,"logicSource");
	physSource = new G4PVPlacement(rotDet,G4ThreeVector(0.,-39.95, 0.),logicSource, "physSource", logicWorld, false, 0, true);

}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{			
	solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
	
	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	
	physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, true);
	
	if(isCherenkov)
		ConstructCherenkov();
		
	if(isScintillator)
		ConstructScintillator();

	if(isGeDetector)
		GeDetector();
		
	return physWorld;
}	


//void MyDetectorConstruction::ConstructSDandField()
//{
//	MyEventAction *evtAction = new MyEventAction();
//	
//	if(isCherenkov)
//		logicDetector->SetSensitiveDetector(sensDet);
//}
