#pragma once
#include "PuzzleModel.h"
#include "BruteForceChecker.h"
#include "ICandidatesSpace.h"

namespace epuzzle::details
{

    class BruteForceContext
    {
    public:
        BruteForceContext(PuzzleModel&& puzzleModel, bool usePrefiltering)
            : m_model(std::move(puzzleModel))
            , m_checker(m_model.attrTypeCount(), m_model.constraints(), usePrefiltering)
        {
            using namespace std::placeholders;
            // see BruteForceChecker class description
            m_space = ICandidatesSpace::create(m_model.personCount(), m_model.attrTypeCount(),
                std::bind(&BruteForceChecker::prefilterCheck, &m_checker, _1, _2, _3));
        }

        const PuzzleModel& puzzleModel() const { return m_model; }
        const BruteForceChecker& checker() const { return m_checker; }
        const ICandidatesSpace& space() const { return *m_space; }

    private:
        PuzzleModel m_model;
        BruteForceChecker m_checker;
        std::unique_ptr<ICandidatesSpace> m_space;
    };

}
