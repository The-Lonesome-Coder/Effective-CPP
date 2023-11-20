#include <functional>
/**
 * Consider alternatives to virtual functions.
*/

/**
 * The fact that healthValue isn’t declared pure virtual suggests that there is a default algorithm for calculating health.
 * Let’s consider some other ways to approach this problem.
*/
class GameCharacter
{
    public:
        virtual int healthValue() const;
};


/**
 * Some people argues that virtual functions should almost always be private.
 * Adherents to this school would suggest that a better design would retain healthValue as a
 * public member function but make it non-virtual and have it call a private virtual function to do the real work.
 *
 * This basic design — having clients call private virtual functions indirectly through public
 * non-virtual member functions — is known as the non-virtual interface (NVI) idiom.
 * It’s a particular manifestation of the more general design pattern called Template Method.
 *
 * An advantage of the NVI idiom is suggested by the “do ‘before’ stuff” and “do ‘after’ stuff” comments in the code.
 * Those comments identify code segments guaranteed to be called before and after the virtual function
 * that does the real work.
*/
class GameCharacter2
{
    public:
        int healthValue() const
        {
            /* ... Do "before" stuff ... */

            int retVal { doHealthValue() };

            /* ... Do "after" stuff ... */

            return retVal;
        }

    private:
        virtual int doHealthValue() const;
};


/**
 *  A more dramatic design assertion would be to say that calculating a character’s health is
 * independent of the character’s type — that such calculations need not be part of the character at all.
 *
 * By using std::function, it can hold any callable entity whose signature is compatible with what is expected.
*/
class GameCharacter3
{
    public:
        /**
         * HealthCalcFunc is any callable entity that can be called with anything compatible
         * with a GameCharacter and that returns anything compatible with an int; see below for details
         *
         * Its signature is “function taking a const GameCharacter& and return an int.
         *
         * An object of this std::function type (i.e., of type HealthCalcFunc) may hold any callable entity
         * compatible with the target signature.
        */
        using HealthCalcFunc = std::function<int (const GameCharacter3&)>;

        explicit GameCharacter3(HealthCalcFunc hcf)
            : healthFunc { hcf } { }

        int healthValue() const
        {
            return healthFunc(*this);
        }

    private:
        HealthCalcFunc healthFunc;
};


/**
 * Clients now have staggeringly more flexibility in specifying health calculation functions.
*/
// Health calculation function
int calcHealth(const GameCharacter3&);

// Class for health calculation function objects
class HealthCalculator
{
    public:
        int operator () (const GameCharacter3&) const;
};


class GameLevel
{
    public:
        // Health calculation member function, note non-int return type
        float health(const GameCharacter3&) const;
};

class Evil : public GameCharacter3
{
    public:
        explicit Evil(HealthCalcFunc hcf)
            : GameCharacter3 { hcf } { }
};

// Character using a health calculation function
Evil e1 {calcHealth};

GameLevel currentLevel;

// Character using a health calculation member function
Evil e2 {std::bind(&GameLevel::health, currentLevel, std::placeholders::_1)};


/**
 * A more conventional approach to Strategy would be to make the health-calculation function a virtual member
 * function of a separate health-calculation hierarchy.
 *
 * GameCharacter is the root of an inheritance hierarchy where EvilBadGuy and EyeCandyCharacter are derived classes;
 * HealthCalcFunc is the root of an inheritance hierarchy with derived classes SlowHealthLoser and FastHealthLoser;
 * and each object of type GameCharacter contains a pointer to an object from the HealthCalcFunc hierarchy.
*/
class GameCharacter4;   // Forward declaration

class HealthCalculateFunction
{
    public:
        virtual int calculate(const GameCharacter4& gc) const;
};


class GameCharacter4
{
    public:
        explicit GameCharacter4(HealthCalculateFunction *p_hcf)
            : p_HealthCalc { p_hcf } { }

        int healthValue() const
        {
            return p_HealthCalc->calculate(*this);
        }

    private:
        HealthCalculateFunction *p_HealthCalc;

};



/**
 * The fundamental advice of this Item is to consider alternatives to virtual functions
 * when searching for a design for the problem you’re trying to solve.
 *
 * Use the non-virtual interface idiom (NVI idiom), a form of the Template Method design pattern
 * that wraps public non-virtual member functions around less accessible virtual functions.
 *
 * Replace virtual functions with std::function data members, thus allowing use of any callable
 * entity with a signature compatible with what you need. This, too, is a form of the Strategy design pattern.
 *
 * Replace virtual functions in one hierarchy with virtual functions in another hierarchy.
 * This is the conventional implementation of the Strategy design pattern.
*/