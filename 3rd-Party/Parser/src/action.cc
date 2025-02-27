#include "action.hh"

;

Action::Action(
    const std::string& name,
    const ParameterList* params,
    const PreconditionList* precond,
    const EffectList* effects)
    : _name(name)
    , _params(params->first)
    , _types(params->second)
    , _precond(precond)
    , _effects(effects)
{
}

const std::string& Action::getName() const
{
    return _name;
}

const StringList* Action::getParams() const
{
    return _params;
}

const TypeDict* Action::getTypes() const
{
    return _types;
}

const PreconditionList* Action::getPrecond() const
{
    return _precond;
}

/**
 * Getter for the preconditions filtered.
 * Removes the special '='.
 */
const PreconditionList* Action::getFilteredPrecond() const
{
    auto* preconditions = new PreconditionList();

    for (auto precondition : *_precond) {
        if (precondition->first->getName() != "=")
            preconditions->push_back(precondition);
    }

    return preconditions;
}

const EffectList* Action::getEffects() const
{
    return _effects;
}

Action::~Action()
{
    if (_params)
        delete _params;
    if (_types)
        delete _types;

    for (auto precondition : *_precond) {
        delete precondition;
    }
    if (_precond)
        delete _precond;

    for (auto effect : *_effects) {
        delete effect;
    }
    delete _effects;
}

std::ostream&
operator<<(std::ostream& out, const Action& action)
{
    out << "Action(name:" << action._name << ")" << std::endl;
    if (action._params) {
        out << ">> params:[";
        auto size = action._params->size();
        for (decltype(size) i = 0; i < size; ++i) {
            auto parameter = (*action._params)[i];
            if (i == 0) {
                out << parameter;
            } else {
                out << ", " << parameter;
            }
            if (action._types && action._types->find(parameter) != action._types->end()) {
                out << " - " << action._types->at(parameter);
            }
        }
        out << "]" << std::endl;
    }
    out << ">> precond:[";
    auto size = action._precond->size();
    for (decltype(size) i = 0; i < size; ++i) {
        auto literal = (*action._precond)[i];
        auto predicate = literal->first;
        bool positive = literal->second;
        if (i == 0) {
            if (!positive)
                out << "NOT ";
            out << *predicate;
        } else {
            out << ", ";
            if (!positive)
                out << "NOT ";
            out << *predicate;
        }
    }
    out << "]" << std::endl;
    size = action._effects->size();
    out << ">> effects:[";
    for (decltype(size) i = 0; i < size; ++i) {
        auto literal = (*action._effects)[i];
        auto predicate = literal->first;
        bool positive = literal->second;
        if (i == 0) {
            if (!positive)
                out << "NOT ";
            out << *predicate;
        } else {
            out << ", ";
            if (!positive)
                out << "NOT ";
            out << *predicate;
        }
    }
    out << "])" << std::endl;
    return out;
}
