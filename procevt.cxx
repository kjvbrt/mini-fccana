// std
#include <iostream>
#include <string>
#include <map>

// *nix
#include <getopt.h>

// Podio
#include "podio/Frame.h"
#include "podio/ROOTReader.h"

// Datamodel
#include "edm4hep/ReconstructedParticleCollection.h"

// Root
#include "TH1D.h"
#include "TFile.h"


void printHelp() {
  std::cout << "Usage: procevt [ih]\n"
            << "  -i/--in-file         input file path\n"
            << "  -h/--help            show this help message" << std::endl;
}


int main(int argc, char** argv) {
  // Default input file path
  std::string inFilePath = "/eos/experiment/fcc/ee/generation/DelphesEvents/"
                           "winter2023/IDEA/p8_ee_ZZ_ecm240/"
                           "events_127276631.root";

  // Setup command line options
  const char* const short_opts = "i:h";
  const option long_opts[] = {{"in-file", required_argument, nullptr, 'i'},
                              {"help", no_argument, nullptr, 'h'},
                              {nullptr, no_argument, nullptr, 0}};

  // Parse command line options
  while (true) {
    const auto opt = getopt_long(argc, argv, short_opts, long_opts, 0);

    if (opt == -1) {
      break;
    }

    switch (opt) {
      case 'i':
        inFilePath = optarg;
        break;
      case 'h':
      case '?':
      default:
        printHelp();
        return EXIT_SUCCESS;
    }
  }

  // Setup histogram(s)
  auto muon_pt = TH1D("muon_pt",
                      "Muon p_{T} [GeV]:p_{T} [GeV];N",
                      100, 0., 100.);
  muon_pt.Sumw2();

  // Create Podio reader
  podio::ROOTReader reader;
  reader.openFile(inFilePath);
  std::cout << "INFO: Reading file:      \n" << inFilePath << std::endl;

  unsigned nEvents = reader.getEntries("events");
  std::cout << "INFO: No. entries: " << nEvents << std::endl;

  // Loop over events
  for (unsigned i = 0; i < nEvents; ++i) {
    if (i % 100 == 0) {
      std::cout << "Seen event: " << i << "\n";
    }
    // Load event frame
    const auto frame = podio::Frame(reader.readNextEntry("events"));

    // Loop over all muons
    auto& muons = frame.get<edm4hep::ReconstructedParticleCollection>("Muon");
    for (const auto& muon: muons) {
      muon_pt.Fill(std::sqrt(std::pow(muon.getMomentum().x, 2) +
                             std::pow(muon.getMomentum().y, 2)));
    }
  }

  // Save result to a file
  auto outFile = TFile("hists.root", "RECREATE");

  muon_pt.Write();

  outFile.Close();
}
