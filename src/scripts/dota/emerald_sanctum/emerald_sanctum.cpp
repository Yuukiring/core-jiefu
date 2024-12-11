#include "scriptPCH.h"
#include "emerald_sanctum.h"

enum
{
    SPELL_POISON_BOLT   = 34218,
    SPELL_CLEAVE        = 34220,
    SPELL_MORTAL_STRIKE = 34221,
    SPELL_WAR_STOMP     = 34222,
    SPELL_REFLECTION    = 34223,
    SPELL_SLEEP         = 34224,
    SPELL_WRATH         = 34225,
    SPELL_ICE_STORM     = 34226,
    SPELL_WING_BUFFET   = 34227,
    SPELL_SHADOW_WORD   = 34228,
    SPELL_REVULSION     = 34229,
    SPELL_RAIN_OF_CHAOS = 34231,
    SAY_AGGRO_WARLOCK   = -2000010,
    SAY_SUMMON_WARLOCK  = -2000011,
    SAY_DEATH_WARLOCK   = -2000012,
};

//npc_542_emerald_dragon_whelp
struct Npc_EmeraldDragonWhelpAI : public ScriptedAI
{
    Npc_EmeraldDragonWhelpAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 POISON_BOLT_TIMER;
    bool HasFled;

    void Reset() override
    {
        POISON_BOLT_TIMER = 500;
        HasFled = false;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(10.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //FLEE
        if (!HasFled && m_creature->GetHealthPercent() < 15.0f)
        {
            HasFled = true;
            m_creature->DoFlee();
            return;
        }
        //POISON BOLT
        if (POISON_BOLT_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_POISON_BOLT);
            POISON_BOLT_TIMER = urand(4000,5000);
        }
        else POISON_BOLT_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_EmeraldDragonWhelpAI(Creature* pCreature)
{
    return new Npc_EmeraldDragonWhelpAI(pCreature);
}

//npc_542_emerald_dragon_guard
struct Npc_EmeraldDragonGuardAI : public ScriptedAI
{
    Npc_EmeraldDragonGuardAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 CLEAVE_TIMER;
    uint32 MORTAL_STRIKE_TIMER;

    void Reset() override
    {
        CLEAVE_TIMER = 5000;
        MORTAL_STRIKE_TIMER = 8000;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(15.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //CLEAVE
        if (CLEAVE_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_CLEAVE);
            if (m_creature->GetHealthPercent() < 50.0f)
                CLEAVE_TIMER = urand(4000,6000);
            else
                CLEAVE_TIMER = urand(5500,9500);
        }
        else CLEAVE_TIMER -= uiDiff;

        //MORTAL STRIKE
        if (MORTAL_STRIKE_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_MORTAL_STRIKE);
            if (m_creature->GetHealthPercent() < 25.0f)
                MORTAL_STRIKE_TIMER = urand(7000,9000);
            else
                MORTAL_STRIKE_TIMER = urand(10000,14000);
        }
        else MORTAL_STRIKE_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_EmeraldDragonGuardAI(Creature* pCreature)
{
    return new Npc_EmeraldDragonGuardAI(pCreature);
}

//npc_542_emerald_drakonid
struct Npc_EmeraldDrakonidAI : public ScriptedAI
{
    Npc_EmeraldDrakonidAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 WAR_STOMP_TIMER;
    bool reflection;
    bool reflection_again;
    bool reflection_and_again;

    void Reset() override
    {
        WAR_STOMP_TIMER = 10000;
        reflection = false;
        reflection_again = false;
        reflection_and_again = false;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(20.0f);
    }

    void AssignRandomThreat()
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, nullptr, SELECT_FLAG_PLAYER))
        {
            DoResetThreat();
            m_creature->GetThreatManager().addThreatDirectly(pTarget, urand(1000, 2000));
        }
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //REFLECTION
        if (m_creature->GetHealthPercent() < 75.0f && !reflection && !m_creature->HasAura(SPELL_REFLECTION))
        {
            DoCastSpellIfCan(m_creature, SPELL_REFLECTION);
            reflection = true;
        }

        if (m_creature->GetHealthPercent() < 50.0f && !reflection_again && !m_creature->HasAura(SPELL_REFLECTION))
        {
            DoCastSpellIfCan(m_creature, SPELL_REFLECTION);
            reflection_again = true;
        }

        if (m_creature->GetHealthPercent() < 25.0f && !reflection_and_again && !m_creature->HasAura(SPELL_REFLECTION))
        {
            DoCastSpellIfCan(m_creature, SPELL_REFLECTION);
            reflection_and_again = true;
        }

        //WAR STOMP
        if (WAR_STOMP_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_WAR_STOMP);
            AssignRandomThreat();
            WAR_STOMP_TIMER = urand(14000,16000);
        }
        else WAR_STOMP_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_EmeraldDrakonidAI(Creature* pCreature)
{
    return new Npc_EmeraldDrakonidAI(pCreature);
}

//npc_542_emerald_wyrmkin
struct Npc_EmeraldWyrmkinAI : public ScriptedAI
{
    Npc_EmeraldWyrmkinAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 SLEEP_TIMER;
    uint32 WRATH_TIMER;
    bool HasFled;

    void Reset() override
    {
        SLEEP_TIMER = 5000;
        WRATH_TIMER = 1000;
        HasFled = false;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(15.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //FLEE
        if (!HasFled && m_creature->GetHealthPercent() < 15.0f)
        {
            HasFled = true;
            m_creature->DoFlee();
            return;
        }
        //SLEEP
        if (SLEEP_TIMER < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_NEAREST, 0, nullptr, SELECT_FLAG_PLAYER))
            {
                if (!pTarget->HasAura(SPELL_SLEEP))
                    DoCastSpellIfCan(pTarget, SPELL_SLEEP);
                SLEEP_TIMER = urand(8000,12000);
            }
        }
        else SLEEP_TIMER -= uiDiff;

        //WRATH
        if (WRATH_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_WRATH);
            WRATH_TIMER = urand(4000,6000);
        }
        else WRATH_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_EmeraldWyrmkinAI(Creature* pCreature)
{
    return new Npc_EmeraldWyrmkinAI(pCreature);
}

//npc_542_emerald_drake
struct Npc_EmeraldDrakeAI : public ScriptedAI
{
    Npc_EmeraldDrakeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 ICE_STORM_TIMER;
    uint32 WING_BUFFET_TIMER;

    void Reset() override
    {
        ICE_STORM_TIMER = 8000;
        WING_BUFFET_TIMER = 10000;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(20.0f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //ICE STORM
        if (ICE_STORM_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_ICE_STORM);
            if (m_creature->GetHealthPercent() < 70.0f)
                ICE_STORM_TIMER = urand(5000,7000);
            else
                ICE_STORM_TIMER = urand(6500,9500);
        }
        else ICE_STORM_TIMER -= uiDiff;

        //WING BUFFET
        if (WING_BUFFET_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_WING_BUFFET);
            if (m_creature->GetHealthPercent() < 35.0f)
                WING_BUFFET_TIMER = urand(6500,8500);
            else
                WING_BUFFET_TIMER = urand(8500,11500);
        }
        else WING_BUFFET_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_EmeraldDrakeAI(Creature* pCreature)
{
    return new Npc_EmeraldDrakeAI(pCreature);
}

//boss_warlock
struct Boss_Warlock : public ScriptedAI
{
    Boss_Warlock(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 SHADOW_WORD_TIMER;
    uint32 REVULSION_TIMER;
    bool summon_99;
    bool summon_66;
    bool summon_33;
    bool kill;

    void Reset() override
    {
        SHADOW_WORD_TIMER = 5000;
        REVULSION_TIMER = 20000;
        summon_99 = false;
        summon_66 = false;
        summon_33 = false;
        kill = false;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->CallForHelp(250.0f);
        DoScriptText(SAY_AGGRO_WARLOCK, m_creature);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_creature->GetHealthPercent() < 99.0f && !summon_99)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_RAIN_OF_CHAOS);
            m_creature->SummonCreature(NPC_INFERNAL, m_creature->GetVictim()->GetPositionX(), m_creature->GetVictim()->GetPositionY(), m_creature->GetVictim()->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, MINUTE * IN_MILLISECONDS);
            m_creature->SummonCreature(NPC_INFERNAL, m_creature->GetVictim()->GetPositionX(), m_creature->GetVictim()->GetPositionY(), m_creature->GetVictim()->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, MINUTE * IN_MILLISECONDS);
            summon_99 = true;
        }

        if (m_creature->GetHealthPercent() < 66.0f && !summon_66)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_RAIN_OF_CHAOS);
            m_creature->SummonCreature(NPC_INFERNAL, m_creature->GetVictim()->GetPositionX(), m_creature->GetVictim()->GetPositionY(), m_creature->GetVictim()->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, MINUTE * IN_MILLISECONDS);
            DoScriptText(SAY_SUMMON_WARLOCK, m_creature);
            summon_66 = true;
        }

        if (m_creature->GetHealthPercent() < 33.0f && !summon_33)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_RAIN_OF_CHAOS);
            m_creature->SummonCreature(NPC_INFERNAL, m_creature->GetVictim()->GetPositionX(), m_creature->GetVictim()->GetPositionY(), m_creature->GetVictim()->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, MINUTE * IN_MILLISECONDS);
            DoScriptText(SAY_SUMMON_WARLOCK, m_creature);
            summon_33 = true;
        }

        if (m_creature->GetHealthPercent() < 10.0f && !kill)
        {
            DoScriptText(SAY_DEATH_WARLOCK, m_creature);
            kill = true;
        }

        //SHADOW WORD
        if (SHADOW_WORD_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_SHADOW_WORD);
            SHADOW_WORD_TIMER = urand(39000,41000);
        }
        else SHADOW_WORD_TIMER -= uiDiff;

        //REVULSION
        if (REVULSION_TIMER < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_REVULSION);
            REVULSION_TIMER = urand(39000,41000);
        }
        else REVULSION_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Boss_Warlock(Creature* pCreature)
{
    return new Boss_Warlock(pCreature);
}

//npc_542_infernal
struct Npc_InfernalAI : public ScriptedAI
{
    Npc_InfernalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 Refresh_Threat_TIMER;

    void Reset() override
    {
        Refresh_Threat_TIMER = 10000;
    }

    void Aggro(Unit* pWho) override
    {
        m_creature->SetInCombatWithZone();
    }

    void AssignRandomThreat()
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, nullptr, SELECT_FLAG_PLAYER))
        {
            DoResetThreat();
            m_creature->GetThreatManager().addThreatDirectly(pTarget, urand(1000, 2000));
        }
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        //Refresh Threat
        if (Refresh_Threat_TIMER < uiDiff)
        {
            AssignRandomThreat();
            Refresh_Threat_TIMER = 10000;
        }
        else Refresh_Threat_TIMER -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_Npc_InfernalAI(Creature* pCreature)
{
    return new Npc_InfernalAI(pCreature);
}

void AddSC_emerald_sanctum()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_542_emerald_dragon_whelp";
    newscript->GetAI = &GetAI_Npc_EmeraldDragonWhelpAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_542_emerald_dragon_guard";
    newscript->GetAI = &GetAI_Npc_EmeraldDragonGuardAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_542_emerald_drakonid";
    newscript->GetAI = &GetAI_Npc_EmeraldDrakonidAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_542_emerald_wyrmkin";
    newscript->GetAI = &GetAI_Npc_EmeraldWyrmkinAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_542_emerald_drake";
    newscript->GetAI = &GetAI_Npc_EmeraldDrakeAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_warlock";
    newscript->GetAI = &GetAI_Boss_Warlock;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_542_infernal";
    newscript->GetAI = &GetAI_Npc_InfernalAI;
    newscript->RegisterSelf();
}
