#ifndef SEAPLUSPLUSENGINE_H
#define SEAPLUSPLUSENGINE_H

#include <memory> // For std::unique_ptr / std::shared_ptr

// Forward declarations to minimize header dependencies
class VertebrateChecker;
class InvertebrateChecker;
class App; // If needed for back-reference/notifications
struct SeaCreatureData;

class SeaPlusPlusEngine {
private:
    // Using unique_ptr assuming the Engine owns the checkers
    std::unique_ptr<VertebrateChecker> vertebrateChecker;
    std::unique_ptr<InvertebrateChecker> invertebrateChecker;
    App* app; // Raw pointer for non-owning back-reference to App (optional)

public:
    // Constructor might take ownership of checkers
    SeaPlusPlusEngine(
        std::unique_ptr<VertebrateChecker> vChecker,
        std::unique_ptr<InvertebrateChecker> iChecker);

    // Destructor needs to be defined if using unique_ptr with forward declared types
    ~SeaPlusPlusEngine();

    // Copy/Move semantics (rule of 5 if needed, or disable)
    SeaPlusPlusEngine(const SeaPlusPlusEngine&) = delete;
    SeaPlusPlusEngine& operator=(const SeaPlusPlusEngine&) = delete;
    SeaPlusPlusEngine(SeaPlusPlusEngine&&) = default; // May need custom definition
    SeaPlusPlusEngine& operator=(SeaPlusPlusEngine&&) = default; // May need custom definition


    // Process the catch details (mediator logic)
    bool processCatch(const SeaCreatureData& creatureData);

    // Optional: Method to register the App (if notifications are needed)
    void registerApp(App* appInstance);

    // Method to explicitly register checkers (alternative to constructor injection)
    // void registerCheckers(std::unique_ptr<VertebrateChecker> vChecker,
    //                      std::unique_ptr<InvertebrateChecker> iChecker);

};

#endif // SEAPLUSPLUSENGINE_H