#pragma once
#include "PuzzleModel.h"
#include "Validator.h"
#include "ICandidatesSpace.h"

namespace epuzzle::details::bruteforce
{

    class SolverContext
    {
    public:
        SolverContext(PuzzleModel&& puzzleModel, bool usePrefiltering)
            : m_model(std::move(puzzleModel))
            , m_validator(m_model.attrTypeCount(), m_model.constraints(), usePrefiltering)
        {
            using namespace std::placeholders;
            // see Validator class description
            m_space = ICandidatesSpace::create(m_model.personCount(), m_model.attrTypeCount(),
                std::bind(&Validator::isAssignmentValid, &m_validator, _1, _2, _3));
        }

        const PuzzleModel& puzzleModel() const { return m_model; }
        const Validator& validator() const { return m_validator; }
        const ICandidatesSpace& searchSpace() const { return *m_space; }

    private:
        PuzzleModel m_model;
        Validator m_validator;
        std::unique_ptr<ICandidatesSpace> m_space;
    };

}
