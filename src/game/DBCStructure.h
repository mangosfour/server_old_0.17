/**
 * MaNGOS is a full featured server for World of Warcraft, supporting
 * the following clients: 1.12.x, 2.4.3, 3.3.5a, 4.3.4a and 5.4.8
 *
 * Copyright (C) 2005-2014  MaNGOS project <http://getmangos.eu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * World of Warcraft, and all World of Warcraft or Warcraft art, images,
 * and lore are copyrighted by Blizzard Entertainment, Inc.
 */

#ifndef MANGOS_DBCSTRUCTURE_H
#define MANGOS_DBCSTRUCTURE_H

#include "Common.h"
#include "DBCEnums.h"
#include "Path.h"
#include "Platform/Define.h"
#include "SharedDefines.h"

#include <map>
#include <set>
#include <vector>

// Structures using to access raw DBC data and required packing to portability

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push,N), also any gcc version not support it at some platform
#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

typedef char const* const* DBCString;                       //char* DBCStrings[MAX_LOCALE];

struct AchievementEntry
{
    uint32    ID;                                           // 0        m_ID
    uint32    factionFlag;                                  // 1        m_faction -1=all, 0=horde, 1=alliance
    uint32    mapID;                                        // 2        m_instance_id -1=none
    uint32      parentAchievement;                          // 3        m_supercedes its Achievement parent (can`t start while parent uncomplete, use its Criteria if don`t have own, use its progress on begin)
    DBCString   name;                                       // 4        m_title_lang
    DBCString   description;                                // 5        m_description_lang
    uint32      categoryId;                                 // 6        m_category
    uint32      points;                                     // 7        m_points
    uint32      OrderInCategory;                            // 8        m_ui_order
    uint32      flags;                                      // 9        m_flags
    uint32      icon;                                       // 10       m_iconID
    DBCString   titleReward;                                // 11       m_reward_lang
    uint32      count;                                      // 12       m_minimum_criteria - need this count of completed criterias (own or referenced achievement criterias)
    uint32      refAchievement;                             // 13       m_shares_criteria - referenced achievement (counting of all completed criterias)
                                                            // 14       5.x
};

struct AchievementCategoryEntry
{
    uint32    ID;                                           // 0        m_ID
    uint32    parentCategory;                               // 1        m_parent -1 for main category
    DBCString name;                                         // 2        m_name_lang
    uint32    sortOrder;                                    // 3        m_ui_order
};

struct AchievementCriteriaEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  referredAchievement;                            // 1        m_achievement_id
    uint32  requiredType;                                   // 2        m_type
    union
    {
        // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE          = 0
        // TODO: also used for player deaths..
        struct
        {
            uint32  creatureID;                             // 3
            uint32  creatureCount;                          // 4
        } kill_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG                 = 1
        struct
        {
            uint32  bgMapID;                                // 3
            uint32  winCount;                               // 4
            uint32  additionalRequirement1_type;            // 5
            uint32  additionalRequirement1_value;           // 6
            uint32  additionalRequirement2_type;            // 7
            uint32  additionalRequirement2_value;           // 8
        } win_bg;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL            = 5
        struct
        {
            uint32  unused;                                 // 3
            uint32  level;                                  // 4
        } reach_level;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL      = 7
        struct
        {
            uint32  skillID;                                // 3
            uint32  skillLevel;                             // 4
        } reach_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT   = 8
        struct
        {
            uint32  linkedAchievement;                      // 3
        } complete_achievement;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT   = 9
        struct
        {
            uint32  unused;                                 // 3
            uint32  totalQuestCount;                        // 4
        } complete_quest_count;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfDays;                           // 4
        } complete_daily_quest_daily;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11
        struct
        {
            uint32  zoneID;                                 // 3
            uint32  questCount;                             // 4
        } complete_quests_in_zone;

        // ACHIEVEMENT_CRITERIA_TYPE_CURRENCY_EARNED         = 12
        struct
        {
            uint32 currencyId;                              // 3
            uint32 count;                                   // 4
        } currencyEarned;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST   = 14
        struct
        {
            uint32  unused;                                 // 3
            uint32  questCount;                             // 4
        } complete_daily_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND  = 15
        struct
        {
            uint32  mapID;                                  // 3
        } complete_battleground;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP           = 16
        struct
        {
            uint32  mapID;                                  // 3
        } death_at_map;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON       = 18
        struct
        {
            uint32  manLimit;                               // 3
        } death_in_dungeon;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID          = 19
        struct
        {
            uint32  groupSize;                              // 3 can be 5, 10 or 25
        } complete_raid;

        // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE     = 20
        struct
        {
            uint32  creatureEntry;                          // 3
        } killed_by_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING     = 24
        struct
        {
            uint32  unused;                                 // 3
            uint32  fallHeight;                             // 4
        } fall_without_dying;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM            = 26
        struct
        {
            uint32 type;                                    // 3, see enum EnviromentalDamage
        } death_from;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST         = 27
        struct
        {
            uint32  questID;                                // 3
            uint32  questCount;                             // 4
        } complete_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET        = 28
        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2       = 69
        struct
        {
            uint32  spellID;                                // 3
            uint32  spellCount;                             // 4
        } be_spell_target;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL             = 29
        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2            = 110
        struct
        {
            uint32  spellID;                                // 3
            uint32  castCount;                              // 4
        } cast_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE = 30
        struct
        {

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31
            uint32  areaID;                                 // 3 Reference to AreaTable.dbc
            uint32  killCount;                              // 4
        } honorable_kill_at_area;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA              = 32
        struct
        {
            uint32  mapID;                                  // 3 Reference to Map.dbc
        } win_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA             = 33
        struct
        {
            uint32  mapID;                                  // 3 Reference to Map.dbc
        } play_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL            = 34
        struct
        {
            uint32  spellID;                                // 3 Reference to Map.dbc
        } learn_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM               = 36
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } own_item;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA        = 37
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
            uint32  flag;                                   // 5 4=in a row
        } win_rated_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING    = 38
        struct
        {
            uint32  teamtype;                               // 3 {2,3,5}
        } highest_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING= 39
        struct
        {
            uint32  teamtype;                               // 3 {2,3,5}
            uint32  teamrating;                             // 4
        } highest_personal_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL      = 40
        struct
        {
            uint32  skillID;                                // 3
            uint32  skillLevel;                             // 4 apprentice=1, journeyman=2, expert=3, artisan=4, master=5, grand master=6
        } learn_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM               = 41
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } use_item;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM              = 42
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } loot_item;

        // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA           = 43
        struct
        {
            // TODO: This rank is _NOT_ the index from AreaTable.dbc
            uint32  areaReference;                          // 3
        } explore_area;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK               = 44
        struct
        {
            // TODO: This rank is _NOT_ the index from CharTitles.dbc
            uint32  rank;                                   // 3
        } own_rank;

        // ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT          = 45
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfSlots;                          // 4
        } buy_bank_slot;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION        = 46
        struct
        {
            uint32  factionID;                              // 3
            uint32  reputationAmount;                       // 4 Total reputation amount, so 42000 = exalted
        } gain_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION= 47
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfExaltedFactions;                // 4
        } gain_exalted_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP      = 48
        struct
        {
            uint32 unused;                                  // 3
            uint32 numberOfVisits;                          // 4
        } visit_barber;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM        = 49
        // TODO: where is the required itemlevel stored?
        struct
        {
            uint32  itemSlot;                               // 3
            uint32  count;                                  // 4
        } equip_epic_item;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT      = 50
        struct
        {
            uint32  rollValue;                              // 3
            uint32  count;                                  // 4
        } roll_need_on_loot;
        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT      = 51
        struct
        {
            uint32  rollValue;                              // 3
            uint32  count;                                  // 4
        } roll_greed_on_loot;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS               = 52
        struct
        {
            uint32  classID;                                // 3
            uint32  count;                                  // 4
        } hk_class;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE                = 53
        struct
        {
            uint32  raceID;                                 // 3
            uint32  count;                                  // 4
        } hk_race;

        // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE               = 54
        // TODO: where is the information about the target stored?
        struct
        {
            uint32  emoteID;                                // 3 enum TextEmotes
            uint32  count;                                  // 4 count of emotes, always required special target or requirements
        } do_emote;
        // ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE            = 13
        // ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE           = 55
        // ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS      = 56
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
            uint32  flag;                                   // 5 =3 for battleground healing
            uint32  mapid;                                  // 6
        } healing_done;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM             = 57
        struct
        {
            uint32  itemID;                                 // 3
            uint32  count;                                  // 4
        } equip_item;

        // ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD= 62
        struct
        {
            uint32  unused;                                 // 3
            uint32  goldInCopper;                           // 4
        } quest_reward_money;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY             = 67
        struct
        {
            uint32  unused;                                 // 3
            uint32  goldInCopper;                           // 4
        } loot_money;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT         = 68
        struct
        {
            uint32  goEntry;                                // 3
            uint32  useCount;                               // 4
        } use_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL       = 70
        // TODO: are those special criteria stored in the dbc or do we have to add another sql table?
        struct
        {
            uint32  unused;                                 // 3
            uint32  killCount;                              // 4
        } special_pvp_kill;

        // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT     = 72
        struct
        {
            uint32  goEntry;                                // 3
            uint32  lootCount;                              // 4
        } fish_in_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS = 75
        struct
        {
            uint32  skillLine;                              // 3
            uint32  spellCount;                             // 4
        } learn_skillline_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL               = 76
        struct
        {
            uint32  unused;                                 // 3
            uint32  duelCount;                              // 4
        } win_duel;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER          = 96
        struct
        {
            uint32  powerType;                              // 3 mana=0, 1=rage, 3=energy, 6=runic power
        } highest_power;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT           = 97
        struct
        {
            uint32  statType;                               // 3 4=spirit, 3=int, 2=stamina, 1=agi, 0=strength
        } highest_stat;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER     = 98
        struct
        {
            uint32  spellSchool;                            // 3
        } highest_spellpower;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING         = 100
        struct
        {
            uint32  ratingType;                             // 3
        } highest_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE              = 109
        struct
        {
            uint32  lootType;                               // 3 3=fishing, 2=pickpocket, 4=disentchant
            uint32  lootTypeCount;                          // 4
        } loot_type;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE       = 112
        struct
        {
            uint32  skillLine;                              // 3
            uint32  spellCount;                             // 4
        } learn_skill_line;

        // ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL    = 113
        struct
        {
            uint32  unused;                                 // 3
            uint32  killCount;                              // 4
        } honorable_kill;

        struct
        {
            uint32  value;                                  // 3        m_asset_id
            uint32  count;                                  // 4        m_quantity
            uint32  additionalRequirement1_type;            // 5        m_start_event
            uint32  additionalRequirement1_value;           // 6        m_start_asset
            uint32  additionalRequirement2_type;            // 7        m_fail_event
            uint32  additionalRequirement2_value;           // 8        m_fail_asset
        } raw;
    };
    //uint32  unk1;                                         // 9
    DBCString name;                                         // 10       m_description_lang
    uint32  completionFlag;                                 // 11       m_flags
    uint32  timedCriteriaStartType;                         // 12       m_timer_start_event Only appears with timed achievements, seems to be the type of starting a timed Achievement, only type 1 and some of type 6 need manual starting
                                                            //              1: ByEventId(?) (serverside IDs),    2: ByQuestId,   5: ByCastSpellId(?)
                                                            //              6: BySpellIdTarget(some of these are unknown spells, some not, some maybe spells)
                                                            //              7: ByKillNpcId,  9: ByUseItemId
    uint32  timedCriteriaMiscId;                            // 13       m_timer_asset_id Alway appears with timed events, used internally to start the achievement, store
    uint32  timeLimit;                                      // 14       m_timer_time time limit in seconds
    uint32  showOrder;                                      // 15       m_ui_order  also used in achievement shift-links as index in state bitmask
    //uint32 unk2;                                          // 16
    //uint32 moreRequirement[3];                            // 17-19
    //uint32 moreRequirementValue[3];                       // 20-22

    // helpers
    bool IsExplicitlyStartedTimedCriteria() const
    {
        if (!timeLimit)
            return false;

        // in case raw.value == timedCriteriaMiscId in timedCriteriaMiscId stored spellid/itemids for cast/use, so repeating aura start at first cast/use until fails
        return requiredType == ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST || raw.value != timedCriteriaMiscId;
    }
};

struct AreaTableEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  mapid;                                          // 1        m_ContinentID
    uint32  zone;                                           // 2        m_ParentAreaID
    uint32  exploreFlag;                                    // 3        m_AreaBit
    uint32  flags;                                          // 4        m_flags
                                                            // 5        5.x
                                                            // 6        m_SoundProviderPref
                                                            // 7        m_SoundProviderPrefUnderwater
                                                            // 8        m_AmbienceID
                                                            // 9        m_ZoneMusic
                                                            // 10       5.x
                                                            // 11       m_IntroSound
    int32   area_level;                                     // 12       m_ExplorationLevel
    DBCString area_name;                                    // 13       m_AreaName_lang
    uint32  team;                                           // 14       m_factionGroupMask
    uint32  LiquidTypeOverride[4];                          // 15-18    m_liquidTypeID[4]
                                                            // 19       m_minElevation
                                                            // 20       m_ambient_multiplier
                                                            // 21       m_lightid
    //uint32 unk20;                                         // 22       4.0.0
    //uint32 unk21;                                         // 23       4.0.0
    //uint32 unk22;                                         // 24       4.0.0
    //uint32 unk23;                                         // 25       4.0.0
    //uint32 unk24;                                         // 26       4.0.1, may be worldStateId
                                                            // 27       5.x
                                                            // 28       5.x
                                                            // 29       5.x
};

struct AreaGroupEntry
{
    uint32  AreaGroupId;                                    // 0        m_ID
    uint32  AreaId[6];                                      // 1-6      m_areaID
    uint32  nextGroup;                                      // 7        m_nextAreaID
};

struct AreaTriggerEntry
{
    uint32  id;                                             // 0        m_ID
    uint32  mapid;                                          // 1        m_ContinentID
    float   x;                                              // 2        m_x
    float   y;                                              // 3        m_y
    float   z;                                              // 4        m_z
    //uint32                                                // 5
    //uint32                                                // 6
    //uint32                                                // 7
    float   radius;                                         // 8        m_radius
    float   box_x;                                          // 9        m_box_length
    float   box_y;                                          // 10       m_box_width
    float   box_z;                                          // 11       m_box_heigh
    float   box_orientation;                                // 12       m_box_yaw
                                                            // 13       5.x
                                                            // 14       5.x
                                                            // 15       5.x
};

struct ArmorLocationEntry
{
  uint32    InventoryType;                                  // 0
  float     Value[5];                                       // 1-5 multiplier for armor types (cloth...plate, no armor?)
};

struct AuctionHouseEntry
{
    uint32    houseId;                                      // 0        m_ID
    uint32    faction;                                      // 1        m_factionID
    uint32    depositPercent;                               // 2        m_depositRate
    uint32    cutPercent;                                   // 3        m_consignmentRate
    //char*     name;                                       // 4        m_name_lang
};

struct BankBagSlotPricesEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  price;                                          // 1        m_Cost
};

struct BarberShopStyleEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  type;                                           // 1        m_type
    //char*   name;                                         // 2        m_DisplayName_lang
    //uint32  unk_name;                                     // 3        m_Description_lang
    //float   CostMultiplier;                               // 4        m_Cost_Modifier
    uint32  race;                                           // 5        m_race
    uint32  gender;                                         // 6        m_sex
    uint32  hair_id;                                        // 7        m_data (real ID to hair/facial hair)
};

struct BattlemasterListEntry
{
    uint32  id;                                             // 0        m_ID
    int32   mapid[16];                                      // 1-16     m_mapID[10]
    uint32  type;                                           // 17       m_instanceType
    //uint32 canJoinAsGroup;                                // 18       m_groupsAllowed
    DBCString name;                                         // 19       m_name_lang
    uint32 maxGroupSize;                                    // 20       m_maxGroupSize
    uint32 HolidayWorldStateId;                             // 21       m_holidayWorldState
    uint32 minLevel;                                        // 22       m_minlevel (sync with PvPDifficulty.dbc content)
    uint32 maxLevel;                                        // 23       m_maxlevel (sync with PvPDifficulty.dbc content)
    uint32 maxGroupSizeRated;                               // 24       4.0.1
    uint32 minPlayers;                                      // 25       4.0.6.13596
    uint32 maxPlayers;                                      // 26       4.0.1
    uint32 rated;                                           // 27       4.0.3, value 2 for Rated Battlegrounds
    //uint32                                                // 28       5.4.1
    //uint32                                                // 29       5.4.1
};

/*struct Cfg_CategoriesEntry
{
    uint32 Index;                                           //          m_ID categoryId (sent in RealmList packet)
    uint32 Unk1;                                            //          m_localeMask
    uint32 Unk2;                                            //          m_charsetMask
    uint32 IsTournamentRealm;                               //          m_flags
    char *categoryName;                                     //          m_name_lang
}*/

/*struct Cfg_ConfigsEntry
{
    uint32 Id;                                              //          m_ID
    uint32 Type;                                            //          m_realmType (sent in RealmList packet)
    uint32 IsPvp;                                           //          m_playerKillingAllowed
    uint32 IsRp;                                            //          m_roleplaying
};*/

#define MAX_OUTFIT_ITEMS 24

struct CharStartOutfitEntry
{
    // uint32 Id;                                           // 0        m_ID
    uint32 RaceClassGender;                                 // 1        m_raceID m_classID m_sexID m_outfitID (UNIT_FIELD_SEX & 0x00FFFFFF) comparable (0 byte = race, 1 byte = class, 2 byte = gender)
    int32 ItemId[MAX_OUTFIT_ITEMS];                         // 2-25     m_ItemID
    // int32 ItemDisplayId[MAX_OUTFIT_ITEMS];               // 26-29    m_DisplayItemID not required at server side
    // int32 ItemInventorySlot[MAX_OUTFIT_ITEMS];           // 50-73    m_InventoryType not required at server side
    // uint32 Unknown1;                                     // 74 unique values (index-like with gaps ordered in other way as ids)
    // uint32 Unknown2;                                     // 75
    // uint32 Unknown3;                                     // 76
    //uint32 Unknown4;                                      // 77
    //uint32 Unknown5;                                      // 78
};

struct CharTitlesEntry
{
    uint32  ID;                                             // 0,       m_ID
    // uint32      unk1;                                    // 1        m_Condition_ID
    DBCString name;                                         // 2        m_name_lang
    //char*       name2;                                    // 3        m_name1_lang
    uint32  bit_index;                                      // 4        m_mask_ID used in PLAYER_CHOSEN_TITLE and 1<<index in PLAYER__FIELD_KNOWN_TITLES
    //uint32                                                // 5
};

struct ChatChannelsEntry
{
    uint32  ChannelID;                                      // 0        m_ID
    uint32  flags;                                          // 1        m_flags
    //uint32                                                // 2        m_factionGroup
    DBCString pattern;                                      // 3        m_name_lang
    //char*       name;                                     // 4        m_shortcut_lang
};

struct ChrClassesEntry
{
    uint32  ClassID;                                        // 0        m_ID
    uint32  powerType;                                      // 1        m_DisplayPower
                                                            // 2        m_petNameToken
    DBCString name;                                         // 3        m_name_lang
    //char*       nameFemale;                               // 4        m_name_female_lang
    //char*       nameNeutralGender;                        // 5        m_name_male_lang
    //char*       capitalizedName                           // 6,       m_filename
    uint32  spellfamily;                                    // 7        m_spellClassSet
    //uint32 flags2;                                        // 8        m_flags (0x08 HasRelicSlot)
    uint32  CinematicSequence;                              // 9        m_cinematicSequenceID
    uint32  apPerStr;                                       // 10       attack power per strength
    uint32  apPerAgi;                                       // 11       attack power per agility
    uint32  rapPerAgi;                                      // 12       ranged attack power per agility
                                                            // 13       5.x
                                                            // 14       5.x
                                                            // 15       5.x
                                                            // 16       5.x
                                                            // 17       5.x
};

struct ChrRacesEntry
{
    uint32      RaceID;                                     // 0        m_ID
                                                            // 1        m_flags
    uint32      FactionID;                                  // 2        m_factionID
                                                            // 3        m_ExplorationSoundID
    uint32      model_m;                                    // 4        m_MaleDisplayId
    uint32      model_f;                                    // 5        m_FemaleDisplayId
                                                            // 6        m_ClientPrefix
    uint32      TeamID;                                     // 7        m_BaseLanguage (7-Alliance 1-Horde)
                                                            // 8        m_creatureType
                                                            // 9        m_ResSicknessSpellID
                                                            // 10       m_SplashSoundID
                                                            // 11       m_clientFileString
    uint32      CinematicSequence;                          // 12       m_cinematicSequenceID
    // uint32    unk_322;                                   // 13       m_alliance (0 alliance, 1 horde, 2 not available?)
    DBCString name;                                         // 14       m_name_lang used for DBC language detection/selection
    //char*       nameFemale;                               // 15       m_name_female_lang
    //char*       nameNeutralGender;                        // 16       m_name_male_lang
                                                            // 17-18    m_facialHairCustomization[2]
                                                            // 19       m_hairCustomization
    uint32      expansion;                                  // 20       m_required_expansion
    //uint32                                                // 21 (23 for worgens)
    //uint32                                                // 22 4.0.0
    //uint32                                                // 23 4.0.0
                                                            // 24-35    5.x
};

struct ChrPowerTypesEntry
{
   uint32 entry;                                               // 0
   uint32 classId;                                             // 1
   uint32 power;                                               // 2
};

/*struct CinematicCameraEntry
{
    uint32      id;                                         // 0        m_ID
    char*       filename;                                   // 1        m_model
    uint32      soundid;                                    // 2        m_soundID
    float       start_x;                                    // 3        m_originX
    float       start_y;                                    // 4        m_originY
    float       start_z;                                    // 5        m_originZ
    float       unk6;                                       // 6        m_originFacing
};*/

struct CinematicSequencesEntry
{
    uint32      Id;                                         // 0        m_ID
    // uint32      unk1;                                    // 1        m_soundID
    // uint32      cinematicCamera;                         // 2        m_camera[8]
};

struct CreatureDisplayInfoEntry
{
    uint32      Displayid;                                  // 0        m_ID
    // 1        m_modelID
    // 2        m_soundID
    uint32      ExtendedDisplayInfoID;                      // 3        m_extendedDisplayInfoID -> CreatureDisplayInfoExtraEntry::DisplayExtraId
    float       scale;                                      // 4        m_creatureModelScale
                                                            // 5        m_creatureModelAlpha
                                                            // 6-8      m_textureVariation[3]
                                                            // 9        m_portraitTextureName
                                                            // 10       m_sizeClass
                                                            // 11       m_bloodID
                                                            // 12       m_NPCSoundID
                                                            // 13       m_particleColorID
                                                            // 14       m_creatureGeosetData
                                                            // 15       m_objectEffectPackageID
                                                            // 16       all 0
                                                            // 17       5.x
                                                            // 18       5.x
                                                            // 19       5.4.1
};

struct CreatureDisplayInfoExtraEntry
{
    uint32      DisplayExtraId;                             // 0        m_ID CreatureDisplayInfoEntry::m_extendedDisplayInfoID
    uint32      Race;                                       // 1        m_DisplayRaceID
    // uint32    Gender;                                    // 2        m_DisplaySexID
    // uint32    SkinColor;                                 // 3        m_SkinID
    // uint32    FaceType;                                  // 4        m_FaceID
    // uint32    HairType;                                  // 5        m_HairStyleID
    // uint32    HairStyle;                                 // 6        m_HairColorID
    // uint32    BeardStyle;                                // 7        m_FacialHairID
    // uint32    Equipment[11];                             // 8-18     m_NPCItemDisplay equipped static items EQUIPMENT_SLOT_HEAD..EQUIPMENT_SLOT_HANDS, client show its by self
    // uint32    CanEquip;                                  // 19       m_flags 0..1 Can equip additional things when used for players
    // char*                                                // 20       m_BakeName CreatureDisplayExtra-*.blp
};

struct CreatureFamilyEntry
{
    uint32  ID;                                             // 0        m_ID
    float   minScale;                                       // 1        m_minScale
    uint32  minScaleLevel;                                  // 2        m_minScaleLevel
    float   maxScale;                                       // 3        m_maxScale
    uint32  maxScaleLevel;                                  // 4        m_maxScaleLevel
    uint32  skillLine[2];                                   // 5-6      m_skillLine
    uint32  petFoodMask;                                    // 7        m_petFoodMask
    int32   petTalentType;                                  // 8        m_petTalentType
                                                            // 9        m_categoryEnumID
    DBCString Name;                                         // 10       m_name_lang
                                                            // 11       m_iconFile
};

#define MAX_CREATURE_SPELL_DATA_SLOT 4

struct CreatureSpellDataEntry
{
    uint32    ID;                                           // 0        m_ID
    uint32    spellId[MAX_CREATURE_SPELL_DATA_SLOT];        // 1-4      m_spells[4]
    // uint32    availability[MAX_CREATURE_SPELL_DATA_SLOT];// 4-7      m_availability[4]
};

struct CreatureTypeEntry
{
    uint32    ID;                                           // 0        m_ID
    //char*   Name;                                         // 1        m_name_lang
    //uint32    no_expirience;                              // 2        m_flags no exp? critters, non-combat pets, gas cloud.
};

/*struct CurrencyCategoryEntry
{
    uint32    ID;                                           // 0        m_ID
    uint32    Unk1;                                         // 1        m_flags 0 for known categories and 3 for unknown one (3.0.9)
    char*   Name;                                           // 2        m_name_lang
};*/

struct CurrencyTypesEntry
{
    uint32  ID;                                             // 0
    uint32 Category;                                        // 1
    DBCString name;                                         // 2
    //char* iconName;                                       // 3
    //char* iconName2;                                      // 4
    //uint32 unk5;                                          // 5
    //uint32 unk6;                                          // 6
    uint32 TotalCap;                                        // 7
    uint32 WeekCap;                                         // 8
    uint32 Flags;                                           // 9
    //uint32                                                // 10       5.4.1
    //DBCString description;                                // 11

    bool HasPrecision() const   { return Flags & CURRENCY_FLAG_HAS_PRECISION; }
    bool HasSeasonCount() const { return Flags & CURRENCY_FLAG_HAS_SEASON_COUNT; }
    float GetPrecision() const  { return HasPrecision() ? CURRENCY_PRECISION : 1.0f; }
};

struct DestructibleModelDataEntry
{
    uint32 m_ID;                                            // 0        m_ID
    uint32 intactDisplayId;                                 // 1
    // uint32 unk2;                                         // 2
    // uint32 unk3;                                         // 3
    // uint32 unk4;                                         // 4
    uint32 damagedDisplayId;                                // 5
    // uint32 unk6;                                         // 6
    // uint32 unk7;                                         // 7
    // uint32 unk8;                                         // 8
    // uint32 unk9;                                         // 9
    uint32 destroyedDisplayId;                              // 10
    // uint32 unk11;                                        // 11
    // uint32 unk12;                                        // 12
    // uint32 unk13;                                        // 13
    // uint32 unk14;                                        // 14
    uint32 rebuildingDisplayId;                             // 15
    // uint32 unk16;                                        // 16
    // uint32 unk17;                                        // 17
    // uint32 unk18;                                        // 18
    // uint32 unk19;                                        // 19
    //uint32 smokeDisplayId;                                // 20
    // uint32 unk21;                                        // 21
    // uint32 unk22;                                        // 22
    // uint32 unk23;                                        // 23
};

struct DungeonEncounterEntry
{
    uint32 Id;                                              // 0        m_ID
    uint32 mapId;                                           // 1        m_mapID
    uint32 Difficulty;                                      // 2        m_difficulty
    uint32 encounterData;                                   // 3        m_orderIndex
    uint32 encounterIndex;                                  // 4        m_Bit
    DBCString encounterName;                                // 5 - encounter name
    //uint32 nameLangFlags;                                 // 6        m_name_lang_flags
    //uint32 spellIconID;                                   // 7        m_spellIconID
    //uint32                                                // 8        5.4.1
};

struct DurabilityCostsEntry
{
    uint32    Itemlvl;                                      // 0        m_ID
    uint32    multiplier[29];                               // 1-29     m_weaponSubClassCost m_armorSubClassCost
};

struct DurabilityQualityEntry
{
    uint32    Id;                                           // 0        m_ID
    float     quality_mod;                                  // 1        m_data
};

struct EmotesEntry
{
    uint32  Id;                                             // 0        m_ID
    //char*   Name;                                         // 1        m_EmoteSlashCommand
    //uint32  AnimationId;                                  // 2        m_AnimID
    uint32  Flags;                                          // 3        m_EmoteFlags
    uint32  EmoteType;                                      // 4        m_EmoteSpecProc (determine how emote are shown)
    uint32  UnitStandState;                                 // 5        m_EmoteSpecProcParam
    //uint32  SoundId;                                      // 6        m_EventSoundID
    //uint32 unk;                                           // 7 - 4.2.0
};

struct EmotesTextEntry
{
    uint32  Id;                                             //          m_ID
                                                            //          m_name
    uint32  textid;                                         //          m_emoteID
                                                            //          m_emoteText
};

struct FactionEntry
{
    uint32      ID;                                         // 0        m_ID
    int32       reputationListID;                           // 1        m_reputationIndex
    uint32      BaseRepRaceMask[4];                         // 2-5      m_reputationRaceMask
    uint32      BaseRepClassMask[4];                        // 6-9      m_reputationClassMask
    int32       BaseRepValue[4];                            // 10-13    m_reputationBase
    uint32      ReputationFlags[4];                         // 14-17    m_reputationFlags
    uint32      team;                                       // 18       m_parentFactionID
    float       spilloverRateIn;                            // 19       m_parentFactionMod[2] Faction gains incoming rep * spilloverRateIn
    float       spilloverRateOut;                           // 20       Faction outputs rep * spilloverRateOut as spillover reputation
    uint32      spilloverMaxRankIn;                         // 21       m_parentFactionCap[2] The highest rank the faction will profit from incoming spillover
    //uint32    spilloverRank_unk;                          // 22       It does not seem to be the max standing at which a faction outputs spillover ...so no idea
    DBCString name;                                         // 23       m_name_lang
    //char*     description;                                // 24       m_description_lang
    //uint32                                                // 25
    //uint32                                                // 26       5.4.1
    //uint32                                                // 27       5.4.1

    // helpers

    int GetIndexFitTo(uint32 raceMask, uint32 classMask) const
    {
        for (int i = 0; i < 4; ++i)
        {
            if ((BaseRepRaceMask[i] == 0 || (BaseRepRaceMask[i] & raceMask)) &&
                    (BaseRepClassMask[i] == 0 || (BaseRepClassMask[i] & classMask)))
                return i;
        }

        return -1;
    }
};

struct FactionTemplateEntry
{
    uint32      ID;                                         // 0        m_ID
    uint32      faction;                                    // 1        m_faction
    uint32      factionFlags;                               // 2        m_flags
    uint32      ourMask;                                    // 3        m_factionGroup
    uint32      friendlyMask;                               // 4        m_friendGroup
    uint32      hostileMask;                                // 5        m_enemyGroup
    uint32      enemyFaction[4];                            // 6        m_enemies[4]
    uint32      friendFaction[4];                           // 10       m_friend[4]
    //-------------------------------------------------------  end structure

    // helpers
    bool IsFriendlyTo(FactionTemplateEntry const& entry) const
    {
        if (entry.faction)
        {
            for (int i = 0; i < 4; ++i)
                if (enemyFaction[i]  == entry.faction)
                    return false;
            for (int i = 0; i < 4; ++i)
                if (friendFaction[i] == entry.faction)
                    return true;
        }
        return (friendlyMask & entry.ourMask) || (ourMask & entry.friendlyMask);
    }
    bool IsHostileTo(FactionTemplateEntry const& entry) const
    {
        if (entry.faction)
        {
            for (int i = 0; i < 4; ++i)
                if (enemyFaction[i]  == entry.faction)
                    return true;
            for (int i = 0; i < 4; ++i)
                if (friendFaction[i] == entry.faction)
                    return false;
        }
        return (hostileMask & entry.ourMask) != 0;
    }
    bool IsHostileToPlayers() const { return (hostileMask & FACTION_MASK_PLAYER) != 0; }
    bool IsNeutralToAll() const
    {
        for (int i = 0; i < 4; ++i)
            if (enemyFaction[i] != 0)
                return false;
        return hostileMask == 0 && friendlyMask == 0;
    }
    bool IsContestedGuardFaction() const { return (factionFlags & FACTION_TEMPLATE_FLAG_CONTESTED_GUARD) != 0; }
};

struct GameObjectDisplayInfoEntry
{
    uint32 Displayid;                                       // 0 m_ID
    char* filename;                                         // 1 m_modelName
    // uint32 unknown2[10];                                 // 2-11 m_Sound
    float geoBoxMinX;                                       // 12 m_geoBoxMinX (use first value as interact dist, mostly in hacks way)
    float geoBoxMinY;                                       // 13 m_geoBoxMinY
    float geoBoxMinZ;                                       // 14 m_geoBoxMinZ
    float geoBoxMaxX;                                       // 15 m_geoBoxMaxX
    float geoBoxMaxY;                                       // 16 m_geoBoxMaxY
    float geoBoxMaxZ;                                       // 17 m_geoBoxMaxZ
    //uint32 transport;                                     // 18
    //uint32 unk;                                           // 19
    //uint32 unk1;                                          // 20
};

struct GemPropertiesEntry
{
    uint32      ID;                                         // 0        m_id
    uint32      spellitemenchantement;                      // 1        m_enchant_id
                                                            // 2        m_maxcount_inv
                                                            // 3        m_maxcount_item
    uint32      color;                                      // 4        m_type
    //uint32                                                // 5
};

struct GlyphPropertiesEntry
{
    uint32  Id;                                             //          m_id
    uint32  SpellId;                                        //          m_spellID
    uint32  TypeFlags;                                      //          m_glyphSlotFlags
    uint32  Unk1;                                           //          m_spellIconID
};

struct GlyphSlotEntry
{
    uint32  Id;                                             //          m_id
    uint32  TypeFlags;                                      //          m_type
    uint32  Order;                                          //          m_tooltip
};

// All Gt* DBC store data for 100 levels, some by 100 per class/race
#define GT_MAX_LEVEL    100
// gtOCTClassCombatRatingScalar.dbc stores data for 32 ratings, look at MAX_COMBAT_RATING for real used amount
#define GT_MAX_RATING   32

struct GtBarberShopCostBaseEntry
{
    //uint32 level;
    float   cost;
};

struct GtCombatRatingsEntry
{
    //uint32 level;
    float    ratio;
};

struct GtChanceToMeleeCritBaseEntry
{
    //uint32 level;
    float    base;
};

struct GtChanceToMeleeCritEntry
{
    //uint32 level;
    float    ratio;
};

struct GtChanceToSpellCritBaseEntry
{
    //uint32 level;
    float    base;
};

struct GtChanceToSpellCritEntry
{
    //uint32 level;
    float    ratio;
};

struct GtOCTClassCombatRatingScalarEntry
{
    float    ratio;
};

struct GtOCTHpPerStaminaEntry
{
    //uint32 level;
    float    ratio;
};

struct GtRegenMPPerSptEntry
{
    //uint32 level;
    float    ratio;
};

struct GtSpellScalingEntry
{
    //uint32 id;
    float value;
};

struct GtOCTBaseHPByClassEntry
{
    float ratio;
};

struct GtOCTBaseMPByClassEntry
{
    float    ratio;
};

/*struct HolidayDescriptionsEntry
{
    uint32 ID;                                              // 0        m_ID this is NOT holiday id
    //char*     name;                                       // 1        m_name_lang
};*/

/*struct HolidayNamesEntry
{
    uint32 ID;                                              // 0,       m_ID this is NOT holiday id
    //char*     name;                                       // 1        m_name_lang
};*/

struct HolidaysEntry
{
    uint32 ID;                                              // 0        m_ID
    // uint32 duration[10];                                 // 1-10     m_duration
    // uint32 date[26];                                     // 11-36    m_date (dates in unix time starting at January, 1, 2000)
    // uint32 region;                                       // 37       m_region (wow region)
    // uint32 looping;                                      // 38       m_looping
    // uint32 calendarFlags[10];                            // 39-48    m_calendarFlags
    // uint32 holidayNameId;                                // 49       m_holidayNameID (HolidayNames.dbc)
    // uint32 holidayDescriptionId;                         // 50       m_holidayDescriptionID (HolidayDescriptions.dbc)
    // char *textureFilename;                               // 51       m_textureFilename
    // uint32 priority;                                     // 52       m_priority
    // uint32 calendarFilterType;                           // 53       m_calendarFilterType (-1,0,1 or 2)
    // uint32 flags;                                        // 54       m_flags
};

struct ItemArmorQualityEntry
{
  uint32    Id;                                             // 0 item level
  float     Value[7];                                       // 1-7 multiplier for item quality
  uint32    Id2;                                            // 8 item level
};

struct ItemArmorShieldEntry
{
  uint32    Id;                                             // 0 item level
  uint32    Id2;                                            // 1 item level
  float     Value[7];                                       // 2-8 multiplier for item quality
};

struct ItemArmorTotalEntry
{
  uint32    Id;                                             // 0 item level
  uint32    Id2;                                            // 1 item level
  float     Value[4];                                       // 2-5 multiplier for armor types (cloth...plate)
};

struct ItemBagFamilyEntry
{
    uint32   ID;                                            // 0        m_ID
    //char*     name;                                       // 1        m_name_lang
};

struct ItemClassEntry
{
    uint32   ID;                                            // 0        m_ID
    //uint32 unk2;                                          // 2 looks like second class
    //uint32 unk3;                                          // 3 1 for weapons
    float PriceFactor;                                      // 2
    DBCString name;                                         // 5        m_name_lang
};

struct ItemDisplayInfoEntry
{
    uint32      ID;                                         // 0        m_ID
                                                            // 1        m_modelName[2]
                                                            // 2        m_modelTexture[2]
                                                            // 3        m_inventoryIcon
                                                            // 4        m_geosetGroup[3]
                                                            // 5        m_flags
                                                            // 6        m_spellVisualID
                                                            // 7        m_groupSoundIndex
                                                            // 8        m_helmetGeosetVis[2]
                                                            // 9        m_texture[2]
                                                            // 10       m_itemVisual[8]
                                                            // 11       m_particleColorID
};

// common struct for:
// ItemDamageAmmo.dbc
// ItemDamageOneHand.dbc
// ItemDamageOneHandCaster.dbc
// ItemDamageRanged.dbc
// ItemDamageThrown.dbc
// ItemDamageTwoHand.dbc
// ItemDamageTwoHandCaster.dbc
// ItemDamageWand.dbc
struct ItemDamageEntry
{
  uint32    Id;                                             // 0 item level
  float     Value[7];                                       // 1-7 multiplier for item quality
  uint32    Id2;                                            // 8 item level
};

struct ItemLimitCategoryEntry
{
    uint32      ID;                                         // 0 Id     m_ID
    //char*     name;                                       // 1        m_name_lang
    uint32      maxCount;                                   // 2,       m_quantity max allowed equipped as item or in gem slot
    uint32      mode;                                       // 3,       m_flags 0 = have, 1 = equip (enum ItemLimitCategoryMode)
};

struct ItemRandomPropertiesEntry
{
    uint32    ID;                                           // 0        m_ID
    //char*     internalName                                // 1        m_Name
    uint32    enchant_id[5];                                // 2-6      m_Enchantment
    DBCString nameSuffix;                                   // 7        m_name_lang
};

struct ItemRandomSuffixEntry
{
    uint32    ID;                                           // 0        m_ID
    DBCString nameSuffix;                                   // 1        m_name_lang
                                                            // 2        m_internalName
    uint32    enchant_id[5];                                // 3-7      m_enchantment
    uint32    prefix[5];                                    // 8-12     m_allocationPct
};

struct ItemReforgeEntry
{
    uint32 Id;                                              // 0
    uint32 SourceStat;                                      // 1
    float SourceMultiplier;                                 // 2
    uint32 FinalStat;                                       // 3
    float FinalMultiplier;                                  // 4
};

struct ItemSetEntry
{
    //uint32    id                                          // 0        m_ID
    DBCString name;                                         // 1        m_name_lang
    //uint32    itemId[17];                                 // 2-18     m_itemID
    uint32    spells[8];                                    // 19-26    m_setSpellID
    uint32    items_to_triggerspell[8];                     // 27-34    m_setThreshold
    uint32    required_skill_id;                            // 35       m_requiredSkill
    uint32    required_skill_value;                         // 36       m_requiredSkillRank
};

/*struct LfgDungeonsEntry
{
    m_ID
    m_name_lang
    m_minLevel
    m_maxLevel
    m_target_level
    m_target_level_min
    m_target_level_max
    m_mapID
    m_difficulty
    m_flags
    m_typeID
    m_faction
    m_textureFilename
    m_expansionLevel
    m_order_index
    m_group_id
    m_description_lang
};*/

/*struct LfgDungeonGroupEntry
{
    m_ID
    m_name_lang
    m_order_index
    m_parent_group_id
    m_typeid
};*/

/*struct LfgDungeonExpansionEntry
{
    m_ID
    m_lfg_id
    m_expansion_level
    m_random_id
    m_hard_level_min
    m_hard_level_max
    m_target_level_min
    m_target_level_max
};*/

struct LiquidTypeEntry
{
    uint32 Id;                                              // 0
    //char* Name;                                           // 1
    //uint32 Flags;                                         // 2 Water: 1|2|4|8, Magma: 8|16|32|64, Slime: 2|64|256, WMO Ocean: 1|2|4|8|512
    uint32 Type;                                            // 3 0: Water, 1: Ocean, 2: Magma, 3: Slime
    //uint32 SoundId;                                       // 4 Reference to SoundEntries.dbc
    uint32 SpellId;                                         // 5 Reference to Spell.dbc
    //float MaxDarkenDepth;                                 // 6 Only oceans got values here!
    //float FogDarkenIntensity;                             // 7 Only oceans got values here!
    //float AmbDarkenIntensity;                             // 8 Only oceans got values here!
    //float DirDarkenIntensity;                             // 9 Only oceans got values here!
    //uint32 LightID;                                       // 10 Only Slime (6) and Magma (7)
    //float ParticleScale;                                  // 11 0: Slime, 1: Water/Ocean, 4: Magma
    //uint32 ParticleMovement;                              // 12
    //uint32 ParticleTexSlots;                              // 13
    //uint32 LiquidMaterialID;                              // 14
    //char* Texture[6];                                     // 15-20
    //uint32 Color[2];                                      // 21-22
    //float Unk1[18];                                       // 23-40 Most likely these are attributes for the shaders. Water: (23, TextureTilesPerBlock),(24, Rotation) Magma: (23, AnimationX),(24, AnimationY)
    //uint32 Unk2[4];                                       // 41-44
};

#define MAX_LOCK_CASE 8

struct LockEntry
{
    uint32      ID;                                         // 0        m_ID
    uint32      Type[MAX_LOCK_CASE];                        // 1-8      m_Type
    uint32      Index[MAX_LOCK_CASE];                       // 9-16     m_Index
    uint32      Skill[MAX_LOCK_CASE];                       // 17-24    m_Skill
    //uint32      Action[MAX_LOCK_CASE];                    // 25-32    m_Action
};

struct MailTemplateEntry
{
    uint32      ID;                                         // 0        m_ID
    //char*       subject;                                  // 1        m_subject_lang
    DBCString content;                                      // 2        m_body_lang
};

struct MapEntry
{
    uint32  MapID;                                          // 0        m_ID
    DBCString   internalname;                                   // 1        m_Directory
    uint32  map_type;                                       // 2        m_InstanceType
    uint32      mapFlags;                                       // 3        m_Flags (0x100 - CAN_CHANGE_PLAYER_DIFFICULTY)
    uint32      unk4;                                           // 4        4.0.1
    uint32      isPvP;                                          // 5        m_PVP 0 or 1 for battlegrounds (not arenas)
    DBCString   name;                                           // 6        m_MapName_lang
    uint32      linked_zone;                                    // 7        m_areaTableID
    DBCString   hordeIntro;                                     // 8        m_MapDescription0_lang
    DBCString   allianceIntro;                                  // 9        m_MapDescription1_lang
    uint32      multimap_id;                                    // 10       m_LoadingScreenID (LoadingScreens.dbc)
    float       BattlefieldMapIconScale;                        // 11       m_minimapIconScale
    int32       ghost_entrance_map;                             // 12       m_corpseMapID map_id of entrance map in ghost mode (continent always and in most cases = normal entrance)
    float       ghost_entrance_x;                               // 13       m_corpseX entrance x coordinate in ghost mode  (in most cases = normal entrance)
    float       ghost_entrance_y;                               // 14       m_corpseY entrance y coordinate in ghost mode  (in most cases = normal entrance)
    uint32      timeOfDayOverride;                              // 15       m_timeOfDayOverride
    uint32      addon;                                          // 16       m_expansionID
    uint32      unkTime;                                        // 17       m_raidOffset
    uint32      maxPlayers;                                     // 18       m_maxPlayers
    int32       rootPhaseMap;                                   // 19       map with base phasing

    // Helpers
    uint32 Expansion() const { return addon; }

    bool IsDungeon() const { return map_type == MAP_INSTANCE || map_type == MAP_RAID || map_type == MAP_SCENARIO; }
    bool IsNonRaidDungeon() const { return map_type == MAP_INSTANCE || map_type == MAP_SCENARIO; }
    bool Instanceable() const { return map_type == MAP_INSTANCE || map_type == MAP_RAID || map_type == MAP_BATTLEGROUND || map_type == MAP_ARENA || map_type == MAP_SCENARIO; }
    bool IsScenario() const { return map_type == MAP_SCENARIO; } 
    bool IsRaid() const { return map_type == MAP_RAID; }
    bool IsBattleGround() const { return map_type == MAP_BATTLEGROUND; }
    bool IsBattleArena() const { return map_type == MAP_ARENA; }
    bool IsBattleGroundOrArena() const { return map_type == MAP_BATTLEGROUND || map_type == MAP_ARENA; }

    bool IsMountAllowed() const
    {
        return !IsDungeon() ||
               MapID == 209 || MapID == 269 || MapID == 309 || // TanarisInstance, CavernsOfTime, Zul'gurub
               MapID == 509 || MapID == 534 || MapID == 560 || // AhnQiraj, HyjalPast, HillsbradPast
               MapID == 568 || MapID == 580 || MapID == 595 || // ZulAman, Sunwell Plateau, Culling of Stratholme
               MapID == 603 || MapID == 615 || MapID == 616 ||     // Ulduar, The Obsidian Sanctum, The Eye Of Eternity
               MapID == 654 || MapID == 655 || MapID == 656 ||     // Gilneas, Gilneas Phase 1, Gilneas Phase 2
               MapID == 658 || MapID == 720 || MapID == 724 ||     // Pit of Saron, Firelands, Ruby Sanctum
               MapID == 754 || MapID == 755 || MapID == 859 ||     // Throne of Four Winds, Tol'vir, Zul'gurub
               MapID == 861 || MapID == 938 || MapID == 939 ||     // Firelands Dailies, End Time, Well of Eternity
               MapID == 940 || MapID == 962 || MapID == 967 ||     // Hour of Twilight, Gate of Setting Sun, Dragon Soul
               MapID == 996 || MapID == 1007 || MapID == 1011;     // Endless Spring, New Scholomance, Niuzao Temple 
    }

    bool IsContinent() const
    {
        return MapID == 0 || MapID == 1 || MapID == 530 || MapID == 571 || MapID == 870;
    }

    bool IsTransport() const
    {
        if (IsContinent())
            return false;
        return map_type == MAP_COMMON && mapFlags == MAP_FLAG_INSTANCEABLE;
    }
};

struct MapDifficultyEntry
{
    uint32      Id;                                         // 0        m_ID
    uint32      MapId;                                      // 1        m_mapID
    uint32      Difficulty;                                 // 2        m_difficulty (for arenas: arena slot)
    DBCString   areaTriggerText;                            // 3        m_message_lang (text showed when transfer to map failed)
    uint32      resetTime;                                  // 4,       m_raidDuration in secs, 0 if no fixed reset time
    uint32      maxPlayers;                                 // 5,       m_maxPlayers some heroic versions have 0 when expected same amount as in normal version
    DBCString   difficultyString;                           // 6        m_difficultystring
};

struct MountCapabilityEntry
{
    uint32 Id;
    uint32 Flags;
    uint32 RequiredRidingSkill;
    uint32 RequiredArea;
    uint32 RequiredAura;
    uint32 RequiredSpell;
    uint32 SpeedModSpell;
    int32  RequiredMap;
};

#define MAX_MOUNT_CAPABILITIES 24

struct MountTypeEntry
{
    uint32 Id;
    uint32 MountCapability[MAX_MOUNT_CAPABILITIES];
};

struct MovieEntry
{
    uint32      Id;                                         // 0        m_ID
    //uint32      unk1;                                     // 1        m_volume
    //uint32      unk2;                                     // 2        5.x
    //uint32      unk3;                                     // 3        4.0.0
    //uint32      unk4;                                     // 4        5.x
};
struct NumTalentsAtLevelEntry
{
    //uint32 Level;                                         // 0 index
    float Talents;                                          // 1 talent count
};

#define MAX_OVERRIDE_SPELLS     10

struct OverrideSpellDataEntry
{
    uint32      Id;                                         // 0        m_ID
    uint32      Spells[MAX_OVERRIDE_SPELLS];                // 1-10     m_spells
    // uint32      unk2;                                    // 11       m_flags
    //uint32      unk3;                                     // 12 possibly flag
};

struct PhaseEntry
{
    uint32 Id;                                              // 0
    uint32 PhaseShift;                                      // 1
    uint32 Flags;                                           // 2 - 0x0, 0x4, 0x8
};

struct PvPDifficultyEntry
{
    //uint32      id;                                       // 0        m_ID
    uint32      mapId;                                      // 1        m_mapID
    uint32      bracketId;                                  // 2        m_rangeIndex
    uint32      minLevel;                                   // 3        m_minLevel
    uint32      maxLevel;                                   // 4        m_maxLevel
    uint32      difficulty;                                 // 5        m_difficulty

    // helpers
    BattleGroundBracketId GetBracketId() const { return BattleGroundBracketId(bracketId); }
};

struct QuestFactionRewardEntry
{
    uint32      id;                                         // 0        m_ID
    int32       rewardValue[10];                            // 1-10     m_Difficulty
};

struct QuestSortEntry
{
    uint32      id;                                         // 0        m_ID
    //char*       name;                                     // 1        m_SortName_lang
};

struct QuestXPLevel
{
    uint32      questLevel;                                 // 0        m_ID
    uint32      xpIndex[10];                                // 1-10     m_difficulty[10]
};

struct RandomPropertiesPointsEntry
{
    uint32    itemLevel;                                    // 0        m_ItemLevel
    uint32    EpicPropertiesPoints[5];                      // 1-5      m_Epic
    uint32    RarePropertiesPoints[5];                      // 6-10     m_Superior
    uint32    UncommonPropertiesPoints[5];                  // 11-15    m_Good
};

struct ScalingStatDistributionEntry
{
    uint32  Id;                                             // 0        m_ID
    int32   StatMod[10];                                    // 1-10     m_statID
    uint32  Modifier[10];                                   // 11-20    m_bonus
    //uint32 unk1;                                          // 21
    uint32  MaxLevel;                                       // 22       m_maxlevel
};

struct ScalingStatValuesEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  Level;                                          // 1        m_charlevel
    uint32  dpsMod[6];                                      // 2-7      DPS mod for level
                                                            // 8        5.x unk bonus
    uint32  spellBonus;                                     // 9        spell power for level
    uint32  ssdMultiplier[5];                               // 10-14     Multiplier for ScalingStatDistribution
    uint32  armorMod[4];                                    // 15-18    Armor for level
    uint32  armorMod2[4];                                   // 19-22    Armor for level
    //uint32 trash[24];                                     // 23-47
    //uint32 unk2;                                          // 48       unk, probably also Armor for level (flag 0x80000?)

    /*struct ScalingStatValuesEntry
    {
        m_ID
        m_charlevel
        m_shoulderBudget
        m_trinketBudget
        m_weaponBudget1H
        m_rangedBudget
        m_clothShoulderArmor
        m_leatherShoulderArmor
        m_mailShoulderArmor
        m_plateShoulderArmor
        m_weaponDPS1H
        m_weaponDPS2H
        m_spellcasterDPS1H
        m_spellcasterDPS2H
        m_rangedDPS
        m_wandDPS
        m_spellPower
        m_primaryBudget
        m_tertiaryBudget
        m_clothCloakArmor
        m_clothChestArmor
        m_leatherChestArmor
        m_mailChestArmor
        m_plateChestArmor
    };*/
    uint32  getssdMultiplier(uint32 mask) const
    {
        if (mask & 0x4001F)
        {
            if(mask & 0x00000001) return ssdMultiplier[1];
            if(mask & 0x00000002) return ssdMultiplier[2]; // 0 and 1 were duplicated
            if(mask & 0x00000004) return ssdMultiplier[3];
            if(mask & 0x00000008) return ssdMultiplier[0];
            if(mask & 0x00000010) return ssdMultiplier[4];
            if(mask & 0x00040000) return ssdMultiplier[2]; // 4.0.0
        }
        return 0;
    }

    uint32  getArmorMod(uint32 mask) const
    {
        if (mask & 0x00F001E0)
        {
            if (mask & 0x00000020) return armorMod[0];
            if (mask & 0x00000040) return armorMod[1];
            if (mask & 0x00000080) return armorMod[2];
            if (mask & 0x00000100) return armorMod[3];

            if (mask & 0x00100000) return armorMod2[0];     // cloth
            if (mask & 0x00200000) return armorMod2[1];     // leather
            if (mask & 0x00400000) return armorMod2[2];     // mail
            if (mask & 0x00800000) return armorMod2[3];     // plate
        }
        return 0;
    }

    uint32 getDPSMod(uint32 mask) const
    {
        if (mask & 0x7E00)
        {
            if (mask & 0x00000200) return dpsMod[0];
            if (mask & 0x00000400) return dpsMod[1];
            if (mask & 0x00000800) return dpsMod[2];
            if (mask & 0x00001000) return dpsMod[3];
            if (mask & 0x00002000) return dpsMod[4];
            if (mask & 0x00004000) return dpsMod[5];        // not used?
        }
        return 0;
    }

    uint32 getSpellBonus(uint32 mask) const
    {
        if (mask & 0x00008000)
            return spellBonus;
        return 0;
    }
};

/*struct SkillLineCategoryEntry{
    uint32    id;                                         // 0      m_ID
    char*     name;                                       // 1      m_name_lang
    uint32    displayOrder;                               // 2      m_sortIndex
};*/

struct SkillRaceClassInfoEntry
{
    //uint32    id;                                         // 0        m_ID
    uint32    skillId;                                      // 1        m_skillID
    uint32    raceMask;                                     // 2        m_raceMask
    uint32    classMask;                                    // 3        m_classMask
    uint32    flags;                                        // 4        m_flags
    uint32    reqLevel;                                     // 5        m_minLevel
    //uint32    skillTierId;                                // 6        m_skillTierID
    //uint32    skillCostID;                                // 7        m_skillCostIndex
};

/*struct SkillTiersEntry{
    uint32    id;                                           // 0        m_ID
    uint32    skillValue[16];                               // 1-17     m_cost
    uint32    maxSkillValue[16];                            // 18-3     m_valueMax
};*/

struct SkillLineEntry
{
    uint32    id;                                           // 0        m_ID
    int32     categoryId;                                   // 1        m_categoryID
    DBCString name;                                         // 2        m_displayName_lang
    //DBCString description;                                // 3        m_description_lang
    uint32    spellIcon;                                    // 4        m_spellIconID
    //DBCString alternateVerb;                              // 5        m_alternateVerb_lang
    uint32    canLink;                                      // 6        m_canLink (prof. with recipes)
                                                            // 7        unk
                                                            // 8        unk
};

struct SkillLineAbilityEntry
{
    uint32    id;                                           // 0        m_ID
    uint32    skillId;                                      // 1        m_skillLine
    uint32    spellId;                                      // 2        m_spell
    uint32    racemask;                                     // 3        m_raceMask
    uint32    classmask;                                    // 4        m_classMask
    uint32    req_skill_value;                              // 7        m_minSkillLineRank
    uint32    forward_spellid;                              // 8        m_supercededBySpell
    uint32    learnOnGetSkill;                              // 9        m_acquireMethod
    uint32    max_value;                                    // 10       m_trivialSkillLineRankHigh
    uint32    min_value;                                    // 11       m_trivialSkillLineRankLow
    //uint32    characterPoints;                            // 12       4.0.0
    //uint32    bonusMaxLevel;                              // 13       4.0.0
                                                            // 12       5.x
};

struct SoundEntriesEntry
{
    uint32          Id;                                     // 0        m_ID
    uint32          Type;                                   // 1        m_soundType
    DBCString       InternalName;                           // 2        m_name
    ///uint32         unk[10];                                // 3-12     unk[10]
    //uint32          unk2[10];                               // 13-22    unk[10]
                                                            // 23       m_volumeFloat
                                                            // 24       m_flags
                                                            // 25       m_minDistance
                                                            // 26       m_distanceCutoff
                                                            // 27       m_EAXDef
                                                            // 28       m_soundEntriesAdvancedID, new in 3.1
                                                            // 29       4.0.0
                                                            // 30       4.0.0
                                                            // 31       4.0.0
                                                            // 32       4.0.0
                                                            // 33       4.3.0
                                                            // 34       5.x
};

struct ClassFamilyMask
{
    uint64 Flags;
    uint32 Flags2;

    ClassFamilyMask() : Flags(0), Flags2(0) {}
    explicit ClassFamilyMask(uint64 familyFlags, uint32 familyFlags2 = 0) : Flags(familyFlags), Flags2(familyFlags2) {}

    bool Empty() const { return Flags == 0 && Flags2 == 0; }
    bool operator! () const { return Empty(); }
    operator void const* () const { return Empty() ? NULL : this; }// for allow normal use in if(mask)

    bool IsFitToFamilyMask(uint64 familyFlags, uint32 familyFlags2 = 0) const
    {
        return (Flags & familyFlags) || (Flags2 & familyFlags2);
    }

    bool IsFitToFamilyMask(ClassFamilyMask const& mask) const
    {
        return (Flags & mask.Flags) || (Flags2 & mask.Flags2);
    }

    uint64 operator& (uint64 mask) const                     // possible will removed at finish convertion code use IsFitToFamilyMask
    {
        return Flags & mask;
    }

    ClassFamilyMask& operator|= (ClassFamilyMask const& mask)
    {
        Flags |= mask.Flags;
        Flags2 |= mask.Flags2;
        return *this;
    }
};

#define MAX_SPELL_TOTEMS 2
#define MAX_SPELL_TOTEM_CATEGORIES 2

// SpellAuraOptions.dbc
struct SpellAuraOptionsEntry
{
    //uint32    Id;                                         // 0        m_ID
                                                            // 1        5.x
                                                            // 2        5.x
    uint32    StackAmount;                                  // 3        m_cumulativeAura
    uint32    procChance;                                   // 4        m_procChance
    uint32    procCharges;                                  // 5        m_procCharges
    uint32    procFlags;                                    // 6        m_procTypeMask
    //uint32                                                // 7        5.4.1
    //uint32                                                // 8        5.4.1
};

// SpellAuraRestrictions.dbc
struct SpellAuraRestrictionsEntry
{
    //uint32    Id;                                         // 0        m_ID
                                                            // 1        5.x
                                                            // 2        5.x
    uint32    CasterAuraState;                              // 21       m_casterAuraState
    uint32    TargetAuraState;                              // 22       m_targetAuraState
    uint32    CasterAuraStateNot;                           // 23       m_excludeCasterAuraState
    uint32    TargetAuraStateNot;                           // 24       m_excludeTargetAuraState
    uint32    casterAuraSpell;                              // 25       m_casterAuraSpell
    uint32    targetAuraSpell;                              // 26       m_targetAuraSpell
    uint32    excludeCasterAuraSpell;                       // 27       m_excludeCasterAuraSpell
    uint32    excludeTargetAuraSpell;                       // 28       m_excludeTargetAuraSpell
};

// SpellCastingRequirements.dbc
struct SpellCastingRequirementsEntry
{
    //uint32    Id;                                           // 0        m_ID
    uint32    FacingCasterFlags;                            // 20       m_facingCasterFlags
    //uint32    MinFactionId;                               // 159      m_minFactionID not used
    //uint32    MinReputation;                              // 160      m_minReputation not used
    int32     AreaGroupId;                                  // 164      m_requiredAreaGroupId
    //uint32    RequiredAuraVision;                         // 161      m_requiredAuraVision not used
    uint32    RequiresSpellFocus;                           // 19       m_requiresSpellFocus
};

// SpellCategories.dbc
struct SpellCategoriesEntry
{
    //uint32    Id;                                         // 0        m_ID
                                                            // 1        5.x
                                                            // 2        5.x
    uint32    Category;                                     // 3        m_category
    uint32    DmgClass;                                     // 4        m_defenseType
    uint32    Dispel;                                       // 5        m_dispelType
    uint32    Mechanic;                                     // 6        m_mechanic
    uint32    PreventionType;                               // 7        m_preventionType
    uint32    StartRecoveryCategory;                        // 8        m_startRecoveryCategory
                                                            // 9        5.x
};

// SpellClassOptions.dbc
struct SpellClassOptionsEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32    modalNextSpell;                             // 50       m_modalNextSpell not used
    ClassFamilyMask SpellFamilyFlags;                       // 149-151  m_spellClassMask NOTE: size is 12 bytes!!!
    uint32    SpellFamilyName;                              // 148      m_spellClassSet
    //char*   Description;                                  // 6 4.0.0

    // helpers

        bool IsFitToFamilyMask(uint64 familyFlags, uint32 familyFlags2 = 0) const
        {
            return SpellFamilyFlags.IsFitToFamilyMask(familyFlags, familyFlags2);
        }

        bool IsFitToFamily(SpellFamily family, uint64 familyFlags, uint32 familyFlags2 = 0) const
        {
            return SpellFamily(SpellFamilyName) == family && IsFitToFamilyMask(familyFlags, familyFlags2);
        }

        bool IsFitToFamilyMask(ClassFamilyMask const& mask) const
        {
            return SpellFamilyFlags.IsFitToFamilyMask(mask);
        }

        bool IsFitToFamily(SpellFamily family, ClassFamilyMask const& mask) const
        {
            return SpellFamily(SpellFamilyName) == family && IsFitToFamilyMask(mask);
        }

    private:
        // catch wrong uses
        template<typename T>
        bool IsFitToFamilyMask(SpellFamily family, T t) const;
};

// SpellCooldowns.dbc
struct SpellCooldownsEntry
{
    //uint32    Id;                                         // 0        m_ID
                                                            // 1        5.x
                                                            // 2        5.x
    uint32    CategoryRecoveryTime;                         // 3        m_categoryRecoveryTime
    uint32    RecoveryTime;                                 // 4        m_recoveryTime
    uint32    StartRecoveryTime;                            // 5        m_startRecoveryTime
};

// SpellEffect.dbc
struct SpellEffectEntry
{
    //uint32    Id;                                           // 0        m_ID
    uint32    Difficulty;                                   // 1        m_difficulty
    uint32    Effect;                                       // 2        m_effect
    float     EffectMultipleValue;                          // 3        m_effectAmplitude
    uint32    EffectApplyAuraName;                          // 4        m_effectAura
    uint32    EffectAmplitude;                              // 5        m_effectAuraPeriod
    int32     EffectBasePoints;                             // 6        m_effectBasePoints (don't must be used in spell/auras explicitly, must be used cached Spell::m_currentBasePoints)
    //float   unk_320_4;                                    // 7        3.2.0
    float     DmgMultiplier;                                // 8        m_effectChainAmplitude
    uint32    EffectChainTarget;                            // 9        m_effectChainTargets
    int32     EffectDieSides;                               // 10       m_effectDieSides
    uint32    EffectItemType;                               // 11       m_effectItemType
    uint32    EffectMechanic;                               // 12       m_effectMechanic
    int32     EffectMiscValue;                              // 13       m_effectMiscValue
    int32     EffectMiscValueB;                             // 14       m_effectMiscValueB
    float     EffectPointsPerComboPoint;                    // 15       m_effectPointsPerCombo
    uint32    EffectRadiusIndex;                            // 16       m_effectRadiusIndex - spellradius.dbc
    uint32    EffectRadiusMaxIndex;                         // 17       4.0.0
    float     EffectRealPointsPerLevel;                     // 18       m_effectRealPointsPerLevel
    ClassFamilyMask EffectSpellClassMask;                   // 19-21    m_effectSpellClassMask
    uint32    EffectSpellClassMask4;                        // 22       5.x FIXME
    uint32    EffectTriggerSpell;                           // 23       m_effectTriggerSpell
    uint32    EffectImplicitTargetA;                        // 24       m_implicitTargetA
    uint32    EffectImplicitTargetB;                        // 25       m_implicitTargetB
    uint32    EffectSpellId;                                // 26       new 4.0.0
    uint32    EffectIndex;                                  // 27       new 4.0.0
    //uint32 unk;                                           // 28       4.2.0

    // helpers

    int32 CalculateSimpleValue() const { return EffectBasePoints; }

    uint32 GetRadiusIndex() const
    {
        if (EffectRadiusIndex != 0)
            return EffectRadiusIndex;

        return EffectRadiusMaxIndex;
    }
};

// SpellEquippedItems.dbc
struct SpellEquippedItemsEntry
{
    //uint32    Id;                                         // 0        m_ID
                                                            // 1        unk 5.1.0
                                                            // 2        unk 5.1.0
    int32     EquippedItemClass;                            // 3        m_equippedItemClass (value)
    int32     EquippedItemInventoryTypeMask;                // 4        m_equippedItemInvTypes (mask)
    int32     EquippedItemSubClassMask;                     // 5        m_equippedItemSubclass (mask)
};

// SpellInterrupts.dbc
struct SpellInterruptsEntry
{
    //uint32    Id;                                         // 0        m_ID
                                                            // 1        unk 5.1.0
                                                            // 2        unk 5.1.0
    uint32    AuraInterruptFlags;                           // 3        m_auraInterruptFlags
    //uint32                                                // 4        4.0.0
    uint32    ChannelInterruptFlags;                        // 5        m_channelInterruptFlags
    //uint32                                                // 6        4.0.0
    uint32    InterruptFlags;                               // 7        m_interruptFlags
};

// SpellLevels.dbc
struct SpellLevelsEntry
{
    //uint32    Id;                                         // 0        m_ID
                                                            // 1        unk 5.1.0
                                                            // 2        unk 5.1.0
    uint32    baseLevel;                                    // 3        m_baseLevel
    uint32    maxLevel;                                     // 4        m_maxLevel
    uint32    spellLevel;                                   // 5        m_spellLevel
};

// SpellPower.dbc
struct SpellPowerEntry
{
    //uint32    Id;                                           // 0 - m_ID
    uint32    SpellId;                                      // 1        5.x
                                                            // 2        5.x
    uint32    powerType;                                    // 3        m_powerType
    uint32    manaCost;                                     // 4        m_manaCost
    uint32    manaCostPerlevel;                             // 5        m_manaCostPerLevel
    uint32    ManaCostPercentage;                           // 6        m_manaCostPct
    uint32    manaPerSecond;                                // 7        m_manaPerSecond
    //uint32  PowerDisplayId;                               // 8        m_powerDisplayID - id from PowerDisplay.dbc, new in 3.1
    //uint32  unk1;                                         // 9        4.0.0
    //unk                                                   // 10       4.3.0
                                                            // 11       5.x
                                                            // 12       5.x
};

// SpellScaling.dbc
struct SpellScalingEntry
{
    //uint32    Id;                                         // 0        m_ID
    int32     castTimeMin;                                  // 1
    int32     castTimeMax;                                  // 2
    int32     castScalingMaxLevel;                          // 3
    int32     playerClass;                                  // 4        (index * 100) + charLevel => gtSpellScaling.dbc
    float     coeff1[3];                                    // 5-7
    float     coeff2[3];                                    // 8-10
    float     coeff3[3];                                    // 11-13
    float     coefBase;                                     // 14       some coefficient, mostly 1.0f
    int32     coefLevelBase;                                // 15       some level

    bool IsScalableEffect(SpellEffectIndex i) const { return coeff1[i] != 0.0f; };
};

// SpellShapeshift.dbc
struct SpellShapeshiftEntry
{
    //uint32    Id;                                           // 0        m_ID
    uint32    StancesNot;                                   // 13       m_shapeshiftMask
    // uint32 unk_320_2;                                    // 14       3.2.0
    uint32    Stances;                                      // 15       m_shapeshiftExclude
    // uint32 unk_320_3;                                    // 16       3.2.0
    // uint32    StanceBarOrder;                            // 155      m_stanceBarOrder not used
};

// SpellTargetRestrictions.dbc
struct SpellTargetRestrictionsEntry
{
    //uint32    Id;                                         // 0        m_ID
                                                            // 1        unk 5.1.0
                                                            // 2        unk 5.1.0
    float     MaxTargetRadius;                              // 3        m_maxTargetRadius
    //float                                                 // 4        unk 5.1.0
    uint32    MaxAffectedTargets;                           // 5        m_maxTargets
    uint32    MaxTargetLevel;                               // 6        m_maxTargetLevel
    uint32    TargetCreatureType;                           // 7        m_targetCreatureType
    uint32    Targets;                                      // 8        m_targets
};

// SpellTotems.dbc
struct SpellTotemsEntry
{
    //uint32    Id;                                           // 0        m_ID
    uint32    TotemCategory[MAX_SPELL_TOTEM_CATEGORIES];    // 162-163  m_requiredTotemCategoryID
    uint32    Totem[MAX_SPELL_TOTEMS];                      // 52-53    m_totem
};

// SpellMisc.dbc
struct SpellMiscEntry
{
    //uint32 Id;                                            // 0
    //uint32 SpellId;                                       // 1
    //uint32                                                // 2        unk
    uint32 Attributes;                                      // 3
    uint32 AttributesEx;                                    // 4
    uint32 AttributesEx2;                                   // 5
    uint32 AttributesEx3;                                   // 6
    uint32 AttributesEx4;                                   // 7
    uint32 AttributesEx5;                                   // 8
    uint32 AttributesEx6;                                   // 9
    uint32 AttributesEx7;                                   // 10
    uint32 AttributesEx8;                                   // 11
    uint32 AttributesEx9;                                   // 12
    uint32 AttributesEx10;                                  // 13
    uint32 AttributesEx11;                                  // 14
    uint32 AttributesEx12;                                  // 15
    uint32 CastingTimeIndex;                                // 16       m_castingTimeIndex
    uint32 DurationIndex;                                   // 17       m_durationIndex
    uint32 rangeIndex;                                      // 18       m_rangeIndex
    float  speed;                                           // 19       m_speed
    uint32 SpellVisual[2];                                  // 20-21    m_spellVisualID
    uint32 SpellIconID;                                     // 22       m_spellIconID
    uint32 activeIconID;                                    // 23       m_activeIconID
    uint32 SchoolMask;                                      // 24       m_schoolMask
};

struct SpellReagentsEntry;

// Spell.dbc
struct MANGOS_DLL_SPEC SpellEntry
{
    uint32    Id;                                           // 0        m_ID
    DBCString SpellName;                                    // 1        m_name_lang
    DBCString Rank;                                         // 2        m_nameSubtext_lang
    //DBCString Description;                                // 3        m_description_lang not used
    //DBCString ToolTip;                                    // 4        m_auraDescription_lang not used
    uint32    runeCostID;                                   // 5        m_runeCostID
    //uint32    spellMissileID;                             // 6        m_spellMissileID not used
    //uint32  spellDescriptionVariableID;                   // 7        m_spellDescriptionVariableID, 3.2.0
    //float unk_f1;                                         // 8 
    uint32 SpellScalingId;                                  // 9        SpellScaling.dbc
    uint32 SpellAuraOptionsId;                              // 10       SpellAuraOptions.dbc
    uint32 SpellAuraRestrictionsId;                         // 11       SpellAuraRestrictions.dbc
    uint32 SpellCastingRequirementsId;                      // 12       SpellCastingRequirements.dbc
    uint32 SpellCategoriesId;                               // 13       SpellCategories.dbc
    uint32 SpellClassOptionsId;                             // 14       SpellClassOptions.dbc
    uint32 SpellCooldownsId;                                // 15       SpellCooldowns.dbc
    uint32 SpellEquippedItemsId;                            // 16       SpellEquippedItems.dbc
    uint32 SpellInterruptsId;                               // 17       SpellInterrupts.dbc
    uint32 SpellLevelsId;                                   // 18       SpellLevels.dbc
    uint32 SpellReagentsId;                                 // 19       SpellReagents.dbc
    uint32 SpellShapeshiftId;                               // 20       SpellShapeshift.dbc
    uint32 SpellTargetRestrictionsId;                       // 21       SpellTargetRestrictions.dbc
    uint32 SpellTotemsId;                                   // 22       SpellTotems.dbc
    //uint32 ResearchProject;                               // 23       ResearchProject.dbc
    uint32 SpellMiscId;                                     // 24       SpellMisc.dbc

    // helpers
    int32 CalculateSimpleValue(SpellEffectIndex eff) const;
    ClassFamilyMask const& GetEffectSpellClassMask(SpellEffectIndex eff) const;

    // struct access functions
    SpellAuraOptionsEntry const* GetSpellAuraOptions() const;
    SpellAuraRestrictionsEntry const* GetSpellAuraRestrictions() const;
    SpellCastingRequirementsEntry const* GetSpellCastingRequirements() const;
    SpellCategoriesEntry const* GetSpellCategories() const;
    SpellClassOptionsEntry const* GetSpellClassOptions() const;
    SpellCooldownsEntry const* GetSpellCooldowns() const;
    SpellEffectEntry const* GetSpellEffect(SpellEffectIndex eff) const;
    SpellEquippedItemsEntry const* GetSpellEquippedItems() const;
    SpellInterruptsEntry const* GetSpellInterrupts() const;
    SpellLevelsEntry const* GetSpellLevels() const;
    SpellPowerEntry const* GetSpellPower() const;
    SpellReagentsEntry const* GetSpellReagents() const;
    SpellScalingEntry const* GetSpellScaling() const;
    SpellShapeshiftEntry const* GetSpellShapeshift() const;
    SpellTargetRestrictionsEntry const* GetSpellTargetRestrictions() const;
    SpellTotemsEntry const* GetSpellTotems() const;
    SpellMiscEntry const* GetSpellMisc() const;

    // single fields
    uint32 GetManaCost() const;
    uint32 GetPreventionType() const;
    uint32 GetCategory() const;
    uint32 GetStartRecoveryTime() const;
    uint32 GetMechanic() const;
    uint32 GetRecoveryTime() const;
    uint32 GetCategoryRecoveryTime() const;
    uint32 GetStartRecoveryCategory() const;
    uint32 GetSpellLevel() const;
    int32 GetEquippedItemClass() const;
    SpellFamily GetSpellFamilyName() const;
    uint32 GetDmgClass() const;
    uint32 GetDispel() const;
    uint32 GetMaxAffectedTargets() const;
    uint32 GetStackAmount() const;
    uint32 GetManaCostPercentage() const;
    uint32 GetProcCharges() const;
    uint32 GetProcChance() const;
    uint32 GetMaxLevel() const;
    uint32 GetTargetAuraState() const;
    uint32 GetManaPerSecond() const;
    uint32 GetRequiresSpellFocus() const;
    uint32 GetSpellEffectIdByIndex(SpellEffectIndex index) const;
    uint32 GetAuraInterruptFlags() const;
    uint32 GetEffectImplicitTargetAByIndex(SpellEffectIndex index) const;
    int32 GetAreaGroupId() const;
    uint32 GetFacingCasterFlags() const;
    uint32 GetBaseLevel() const;
    uint32 GetInterruptFlags() const;
    uint32 GetTargetCreatureType() const;
    int32 GetEffectMiscValue(SpellEffectIndex index) const;
    uint32 GetStances() const;
    uint32 GetStancesNot() const;
    uint32 GetProcFlags() const;
    uint32 GetChannelInterruptFlags() const;
    uint32 GetManaCostPerLevel() const;
    uint32 GetCasterAuraState() const;
    uint32 GetTargets() const;
    uint32 GetEffectApplyAuraNameByIndex(SpellEffectIndex index) const;
    uint32 GetCastingTimeIndex() const;
    uint32 GetDurationIndex() const;
    uint32 GetRangeIndex() const;
    float GetSpeed() const;
    uint32 GetSpellVisual(int idx = 0) const;
    uint32 GetSpellIconID() const;
    uint32 GetActiveIconID() const;
    uint32 GetSchoolMask() const;
    uint32 GetPowerType() const;

    bool IsFitToFamilyMask(uint64 familyFlags, uint32 familyFlags2 = 0) const
    {
        SpellClassOptionsEntry const* classOpt = GetSpellClassOptions();
        return classOpt && classOpt->IsFitToFamilyMask(familyFlags, familyFlags2);
    }

    bool IsFitToFamily(SpellFamily family, uint64 familyFlags, uint32 familyFlags2 = 0) const
    {
        SpellClassOptionsEntry const* classOpt = GetSpellClassOptions();
        return classOpt && classOpt->IsFitToFamily(family, familyFlags, familyFlags2);
    }

    bool IsFitToFamilyMask(ClassFamilyMask const& mask) const
    {
        SpellClassOptionsEntry const* classOpt = GetSpellClassOptions();
        return classOpt && classOpt->IsFitToFamilyMask(mask);
    }

    bool IsFitToFamily(SpellFamily family, ClassFamilyMask const& mask) const
    {
        SpellClassOptionsEntry const* classOpt = GetSpellClassOptions();
        return classOpt && classOpt->IsFitToFamily(family, mask);
    }

    uint32 GetAttributes() const;
    uint32 GetAttributesEx() const;
    uint32 GetAttributesEx2() const;
    uint32 GetAttributesEx3() const;
    uint32 GetAttributesEx4() const;
    uint32 GetAttributesEx5() const;
    uint32 GetAttributesEx6() const;
    uint32 GetAttributesEx7() const;
    uint32 GetAttributesEx8() const;
    uint32 GetAttributesEx9() const;
    uint32 GetAttributesEx10() const;
    uint32 GetAttributesEx11() const;
    uint32 GetAttributesEx12() const;
    inline bool HasAttribute(SpellAttributes attribute) const { return GetAttributes() & attribute; }
    inline bool HasAttribute(SpellAttributesEx attribute) const { return GetAttributesEx() & attribute; }
    inline bool HasAttribute(SpellAttributesEx2 attribute) const { return GetAttributesEx2() & attribute; }
    inline bool HasAttribute(SpellAttributesEx3 attribute) const { return GetAttributesEx3() & attribute; }
    inline bool HasAttribute(SpellAttributesEx4 attribute) const { return GetAttributesEx4() & attribute; }
    inline bool HasAttribute(SpellAttributesEx5 attribute) const { return GetAttributesEx5() & attribute; }
    inline bool HasAttribute(SpellAttributesEx6 attribute) const { return GetAttributesEx6() & attribute; }
    inline bool HasAttribute(SpellAttributesEx7 attribute) const { return GetAttributesEx7() & attribute; }
    inline bool HasAttribute(SpellAttributesEx8 attribute) const { return GetAttributesEx8() & attribute; }
    inline bool HasAttribute(SpellAttributesEx9 attribute) const { return GetAttributesEx9() & attribute; }
    inline bool HasAttribute(SpellAttributesEx10 attribute) const { return GetAttributesEx10() & attribute; }
    inline bool HasAttribute(SpellAttributesEx11 attribute) const { return GetAttributesEx11() & attribute; }
    inline bool HasAttribute(SpellAttributesEx12 attribute) const { return GetAttributesEx12() & attribute; }

    private:
        // prevent creating custom entries (copy data from original in fact)
        SpellEntry(SpellEntry const&);                      // DON'T must have implementation

        // catch wrong uses
        template<typename T>
        bool IsFitToFamilyMask(SpellFamily family, T t) const;
};

struct SpellCastTimesEntry
{
    uint32    ID;                                           // 0        m_ID
    int32     CastTime;                                     // 1        m_base
    float     CastTimePerLevel;                             // 2        m_perLevel
    int32     MinCastTime;                                  // 3        m_minimum
};

struct SpellFocusObjectEntry
{
    uint32    ID;                                           // 0        m_ID
    //char*     Name;                                       // 1        m_name_lang
};

struct SpellRadiusEntry
{
    uint32    ID;                                           // 0        m_ID
    float     Radius;                                       // 1        m_radius
                                                            // 2        m_radiusPerLevel
    //float                                                 // 3        5.x
    //float     RadiusMax;                                  // 4        m_radiusMax
};

struct SpellRangeEntry
{
    uint32    ID;                                           // 0        m_ID
    float     minRange;                                     // 1        m_rangeMin[2]
    float     minRangeFriendly;                             // 2
    float     maxRange;                                     // 3        m_rangeMax[2]
    float     maxRangeFriendly;                             // 4
    //uint32  Flags;                                        // 5        m_flags
    //char*   Name;                                         // 6-21     m_displayName_lang
    //char*   ShortName;                                    // 23-38    m_displayNameShort_lang
};

struct SpellRuneCostEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  RuneCost[3];                                    // 1-3      m_blood m_unholy m_frost (0=blood, 1=frost, 2=unholy)
    //uint32                                                // 4        5.x
    uint32  runePowerGain;                                  // 5        m_runicPower

    bool NoRuneCost() const { return RuneCost[0] == 0 && RuneCost[1] == 0 && RuneCost[2] == 0; }
    bool NoRunicPowerGain() const { return runePowerGain == 0; }
};

struct SpellShapeshiftFormEntry
{
    uint32 ID;                                              // 0        m_ID
    //uint32 buttonPosition;                                // 1        m_bonusActionBar
    //char*  Name;                                          // 2        m_name_lang
    uint32 flags1;                                          // 3        m_flags
    int32  creatureType;                                    // 4        m_creatureType <=0 humanoid, other normal creature types
    //uint32 unk1;                                          // 5        m_attackIconID
    uint32 attackSpeed;                                     // 6        m_combatRoundTime
    uint32 modelID_A;                                       // 7        m_creatureDisplayID[4]
    uint32 modelID_H;                                       // 8
    //uint32 unk3;                                          // 9 unused always 0
    //uint32 unk4;                                          // 10 unused always 0
    uint32 spellId[8];                                      // 11-18    m_presetSpellID[8]
    //uint32 unk5;                                          // 19 unused, !=0 for flight forms
    //uint32 unk6;                                          // 20
};

struct SpellDifficultyEntry
{
    uint32 ID;                                              // 0        m_ID
    uint32 spellId[MAX_DIFFICULTY];                         // 1-4      m_difficultySpellID[4]
};

struct SpellDurationEntry
{
    uint32    ID;                                           //          m_ID
    int32     Duration[3];                                  //          m_duration, m_durationPerLevel, m_maxDuration
};

struct SpellItemEnchantmentEntry
{
    uint32      ID;                                         // 0        m_ID
    //uint32      charges;                                  // 1        m_charges
    uint32      type[3];                                    // 2-4      m_effect[3]
    uint32      amount[3];                                  // 5-7      m_effectPointsMin[3]
    uint32      spellid[3];                                 // 8-10     m_effectArg[3]
    DBCString description;                                  // 11       m_name_lang
    uint32      aura_id;                                    // 12       m_itemVisual
    uint32      slot;                                       // 13       m_flags
    uint32      GemID;                                      // 14       m_src_itemID
    uint32      EnchantmentCondition;                       // 15       m_condition_id
    //uint32      requiredSkill;                            // 16       m_requiredSkillID
    //uint32      requiredSkillValue;                       // 17       m_requiredSkillRank
    uint32      requiredLevel;                              // 18       new in 3.1
    //float                                                 // 19       5.x
    //uint32      minItemLevel;                             // 20       new in 3.1
    //int32                                                 // 21       5.x
    //int32                                                 // 22       5.4.1
    //float                                                 // 23       5.x
    //uint32                                                // 24       5.x
    //uint32                                                // 25       5.x
};

struct SpellItemEnchantmentConditionEntry
{
    uint32  ID;                                             // 0        m_ID
    uint8   Color[5];                                       // 1-5      m_lt_operandType[5]
    //uint32  LT_Operand[5];                                // 6-10     m_lt_operand[5]
    uint8   Comparator[5];                                  // 11-15    m_operator[5]
    uint8   CompareColor[5];                                // 15-20    m_rt_operandType[5]
    uint32  Value[5];                                       // 21-25    m_rt_operand[5]
    //uint8   Logic[5]                                      // 25-30    m_logic[5]
};

struct SummonPropertiesEntry
{
    uint32  Id;                                             // 0        m_id
    uint32  Group;                                          // 1        m_control (enum SummonPropGroup)
    uint32  FactionId;                                      // 2        m_faction
    uint32  Title;                                          // 3        m_title (enum UnitNameSummonTitle)
    uint32  Slot;                                           // 4        m_slot if title = UNITNAME_SUMMON_TITLE_TOTEM, its actual slot (0-6).
                                                            //      if title = UNITNAME_SUMMON_TITLE_COMPANION, slot=6 -> defensive guardian, in other cases criter/minipet
                                                            //      Slot may have other uses, selection of pet type in some cases?
    uint32  Flags;                                          // 5        m_flags (enum SummonPropFlags)
};

#define MAX_TALENT_RANK 5
#define MAX_PET_TALENT_RANK 3                               // use in calculations, expected <= MAX_TALENT_RANK
#define MAX_TALENT_TABS 3

struct TalentEntry
{
    uint32    TalentID;                                     // 0        m_ID
    uint32    TalentTab;                                    // 1        m_tabID (TalentTab.dbc)
    uint32    Row;                                          // 2        m_tierID
    uint32    Col;                                          // 3        m_columnIndex
    uint32    RankID[MAX_TALENT_RANK];                      // 4-6      m_spellRank
    uint32    DependsOn;                                    // 9        m_prereqTalent (Talent.dbc)
                                                            // 10-11 part of prev field
    uint32    DependsOnRank;                                // 12       m_prereqRank
                                                            // 13-14 part of prev field
    //uint32  needAddInSpellBook;                           // 15       m_flags also need disable higest ranks on reset talent tree
    //uint32  unk1;                                         // 16       m_requiredSpellID
    //uint64  allowForPet;                                  // 17       m_categoryMask its a 64 bit mask for pet 1<<m_categoryEnumID in CreatureFamily.dbc
};

struct TalentTabEntry
{
    uint32  TalentTabID;                                    // 0        m_ID
    //char* name;                                           // 1        m_name_lang
    //unit32  spellicon;                                    // 2        m_spellIconID
    uint32  ClassMask;                                      // 3        m_classMask
    uint32  petTalentMask;                                  // 4        m_petTalentMask
    uint32  tabpage;                                        // 5        m_orderIndex
    //char* internalname;                                   // 6        m_backgroundFile
    //char* description;                                    // 7
    uint32 rolesMask;                                       // 8        4.0.0
    uint32 masterySpells[MAX_MASTERY_SPELLS];               // 9-10     passive mastery bonus spells
};

struct TalentTreePrimarySpellsEntry
{
    //uint32 Id;                                            // 0 index
    uint32 TalentTree;                                      // 1 entry from TalentTab.dbc
    uint32 SpellId;                                         // 2 spell id to learn
    //uint32 Flags;                                         // 3 some kind of flags
};

struct TaxiNodesEntry
{
    uint32    ID;                                           // 0        m_ID
    uint32    map_id;                                       // 1        m_ContinentID
    float     x;                                            // 2        m_x
    float     y;                                            // 3        m_y
    float     z;                                            // 4        m_z
    DBCString name;                                         // 5        m_Name_lang
    uint32    MountCreatureID[2];                           // 6-7      m_MountCreatureID[2]
    //uint32                                                // 8        5.4.1
    //uint32 unk;                                           // 9        4.2.0
    //float unk1;                                           // 10       4.2.0
    //float unk2;                                           // 11       4.2.0
};

struct TaxiPathEntry
{
    uint32    ID;                                           // 0        m_ID
    uint32    from;                                         // 1        m_FromTaxiNode
    uint32    to;                                           // 2        m_ToTaxiNode
    uint32    price;                                        // 3        m_Cost
};

struct TaxiPathNodeEntry
{
                                                            // 0        m_ID
    uint32    path;                                         // 1        m_PathID
    uint32    index;                                        // 2        m_NodeIndex
    uint32    mapid;                                        // 3        m_ContinentID
    float     x;                                            // 4        m_LocX
    float     y;                                            // 5        m_LocY
    float     z;                                            // 6        m_LocZ
    uint32    actionFlag;                                   // 7        m_flags
    uint32    delay;                                        // 8        m_delay
    uint32    arrivalEventID;                               // 9        m_arrivalEventID
    uint32    departureEventID;                             // 10       m_departureEventID
};

struct TotemCategoryEntry
{
    uint32    ID;                                           // 0        m_ID
    //char*   name;                                         // 1        m_name_lang
    uint32    categoryType;                                 // 2        m_totemCategoryType (one for specialization)
    uint32    categoryMask;                                 // 3        m_totemCategoryMask (compatibility mask for same type: different for totems, compatible from high to low for rods)
};

struct TransportAnimationEntry
{
    //uint32    id;                                         // 0
    uint32    transportEntry;                               // 1
    uint32    timeFrame;                                    // 2
    //float     xOffs;                                      // 3
    //float     yOffs;                                      // 4
    //float     zOffs;                                      // 5
    //uint32    unk;                                        // 6
};

#define MAX_VEHICLE_SEAT 8

struct VehicleEntry
{
    uint32  m_ID;                                           // 0
    uint32  m_flags;                                        // 1
    //uint32                                                // 2    5.x
    float   m_turnSpeed;                                    // 3
    float   m_pitchSpeed;                                   // 4
    float   m_pitchMin;                                     // 5
    float   m_pitchMax;                                     // 6
    uint32  m_seatID[MAX_VEHICLE_SEAT];                     // 7-14
    float   m_mouseLookOffsetPitch;                         // 15
    float   m_cameraFadeDistScalarMin;                      // 16
    float   m_cameraFadeDistScalarMax;                      // 17
    float   m_cameraPitchOffset;                            // 18
    float   m_facingLimitRight;                             // 19
    float   m_facingLimitLeft;                              // 20
    float   m_msslTrgtTurnLingering;                        // 21
    float   m_msslTrgtPitchLingering;                       // 22
    float   m_msslTrgtMouseLingering;                       // 23
    float   m_msslTrgtEndOpacity;                           // 24
    float   m_msslTrgtArcSpeed;                             // 25
    float   m_msslTrgtArcRepeat;                            // 26
    float   m_msslTrgtArcWidth;                             // 27
    float   m_msslTrgtImpactRadius[2];                      // 28-29
    DBCString m_msslTrgtArcTexture;                         // 30
    DBCString m_msslTrgtImpactTexture;                      // 31
    DBCString m_msslTrgtImpactModel[2];                     // 32-33
    float   m_cameraYawOffset;                              // 34
    uint32  m_uiLocomotionType;                             // 35
    float   m_msslTrgtImpactTexRadius;                      // 36
    uint32  m_uiSeatIndicatorType;                          // 37       m_vehicleUIIndicatorID
    uint32  m_powerType;                                    // 38       m_powerDisplayID
                                                            // 39       new in 3.1
                                                            // 30       new in 3.1
};

struct VehicleSeatEntry
{
    uint32  m_ID;                                           // 0
    uint32  m_flags;                                        // 1
    int32   m_attachmentID;                                 // 2
    float   m_attachmentOffsetX;                            // 3
    float   m_attachmentOffsetY;                            // 4
    float   m_attachmentOffsetZ;                            // 5
    float   m_enterPreDelay;                                // 6
    float   m_enterSpeed;                                   // 7
    float   m_enterGravity;                                 // 8
    float   m_enterMinDuration;                             // 9
    float   m_enterMaxDuration;                             // 10
    float   m_enterMinArcHeight;                            // 11
    float   m_enterMaxArcHeight;                            // 12
    int32   m_enterAnimStart;                               // 13
    int32   m_enterAnimLoop;                                // 14
    int32   m_rideAnimStart;                                // 15
    int32   m_rideAnimLoop;                                 // 16
    int32   m_rideUpperAnimStart;                           // 17
    int32   m_rideUpperAnimLoop;                            // 18
    float   m_exitPreDelay;                                 // 19
    float   m_exitSpeed;                                    // 20
    float   m_exitGravity;                                  // 21
    float   m_exitMinDuration;                              // 22
    float   m_exitMaxDuration;                              // 23
    float   m_exitMinArcHeight;                             // 24
    float   m_exitMaxArcHeight;                             // 25
    int32   m_exitAnimStart;                                // 26
    int32   m_exitAnimLoop;                                 // 27
    int32   m_exitAnimEnd;                                  // 28
    float   m_passengerYaw;                                 // 29
    float   m_passengerPitch;                               // 30
    float   m_passengerRoll;                                // 31
    int32   m_passengerAttachmentID;                        // 32
    int32   m_vehicleEnterAnim;                             // 33
    int32   m_vehicleExitAnim;                              // 34
    int32   m_vehicleRideAnimLoop;                          // 35
    int32   m_vehicleEnterAnimBone;                         // 36
    int32   m_vehicleExitAnimBone;                          // 37
    int32   m_vehicleRideAnimLoopBone;                      // 38
    float   m_vehicleEnterAnimDelay;                        // 39
    float   m_vehicleExitAnimDelay;                         // 40
    uint32  m_vehicleAbilityDisplay;                        // 41
    uint32  m_enterUISoundID;                               // 42
    uint32  m_exitUISoundID;                                // 43
    int32   m_uiSkin;                                       // 44
    uint32  m_flagsB;                                       // 45
                                                            // 46       m_cameraEnteringDelay
                                                            // 47       m_cameraEnteringDuration
                                                            // 48       m_cameraExitingDelay
                                                            // 49       m_cameraExitingDuration
                                                            // 50       m_cameraOffsetX
                                                            // 51       m_cameraOffsetY
                                                            // 52       m_cameraOffsetZ
                                                            // 53       m_cameraPosChaseRate
                                                            // 54       m_cameraFacingChaseRate
                                                            // 55       m_cameraEnteringZoom"
                                                            // 56       m_cameraSeatZoomMin
                                                            // 57       m_cameraSeatZoomMax
    //uint32 unk[6];                                        // 58-63
    //uint32 unk2;                                          // 64 4.0.0
    //uint32 unk3;                                          // 65 4.0.1

    bool IsUsable() const { return
        (m_flags & SEAT_FLAG_USABLE) ||
        (m_flags & SEAT_FLAG_CAN_CONTROL) ||
        (m_flags & SEAT_FLAG_UNCONTROLLED) ||
        (m_flagsB & VEHICLE_SEAT_FLAG_B_USABLE_FORCED) ||
        (m_flagsB & VEHICLE_SEAT_FLAG_B_USABLE_FORCED_2) ||
        (m_flagsB & VEHICLE_SEAT_FLAG_B_USABLE_FORCED_3) ||
        (m_flagsB & VEHICLE_SEAT_FLAG_B_USABLE_FORCED_4); }
};

struct WMOAreaTableEntry
{
    uint32 Id;                                              // 0        m_ID index
    int32 rootId;                                           // 1        m_WMOID used in root WMO
    int32 adtId;                                            // 2        m_NameSetID used in adt file
    int32 groupId;                                          // 3        m_WMOGroupID used in group WMO
    //uint32 field4;                                        // 4        m_SoundProviderPref
    //uint32 field5;                                        // 5        m_SoundProviderPrefUnderwater
    //uint32 field6;                                        // 6        m_AmbienceID
    //uint32 field7;                                        // 7        m_ZoneMusic
    //uint32 field8;                                        // 8        m_IntroSound
    uint32 Flags;                                           // 9        m_flags (used for indoor/outdoor determination)
    uint32 areaId;                                          // 10       m_AreaTableID (AreaTable.dbc)
    //char *Name;                                           // 11       m_AreaName_lang
    //uint32 field12;                                       // 12
    //uint32 field13;                                       // 13
    //uint32 field14;                                       // 14
};

struct WorldMapAreaEntry
{
    //uint32  ID;                                           // 0        m_ID
    uint32  map_id;                                         // 1        m_mapID
    uint32  area_id;                                        // 2        m_areaID index (continent 0 areas ignored)
    //char* internal_name                                   // 3        m_areaName
    float   y1;                                             // 4        m_locLeft
    float   y2;                                             // 5        m_locRight
    float   x1;                                             // 6        m_locTop
    float   x2;                                             // 7        m_locBottom
    int32   virtual_map_id;                                 // 8        m_displayMapID -1 (map_id have correct map) other: virtual map where zone show (map_id - where zone in fact internally)
    // int32   dungeonMap_id;                               // 9        m_defaultDungeonFloor (DungeonMap.dbc)
    // uint32  someMapID;                                   // 10       m_parentWorldMapID
    //uint32   unk1;                                        // 11 4.0.0
    //uint32 unk2;                                          // 12 - 4.3.0
    //uint32 unk4;                                          // 13 - 4.3.0
};

#define MAX_WORLD_MAP_OVERLAY_AREA_IDX 4

struct WorldMapOverlayEntry
{
    uint32    ID;                                           // 0        m_ID
    //uint32    worldMapAreaId;                             // 1        m_mapAreaID (WorldMapArea.dbc)
    uint32    areatableID[MAX_WORLD_MAP_OVERLAY_AREA_IDX];  // 2-5      m_areaID
    //char* internal_name                                   // 6        m_textureName
                                                            // 7        m_textureWidth
                                                            // 8        m_textureHeight
                                                            // 9        m_offsetX
                                                            // 10       m_offsetY
                                                            // 11       m_hitRectTop
                                                            // 12       m_hitRectLeft
                                                            // 13       m_hitRectBottom
                                                            // 14       m_hitRectRight
                                                            // 15       5.x
};

struct WorldSafeLocsEntry
{
    uint32    ID;                                           // 0        m_ID
    uint32    map_id;                                       // 1        m_continent
    float     x;                                            // 2        m_locX
    float     y;                                            // 3        m_locY
    float     z;                                            // 4        m_locZ
    //float                                                 // 5        5.x
    //char*   name;                                         // 6        m_AreaName_lang
};

struct WorldPvPAreaEntry
{
    uint32      Id;                                         // 0 m_battlefieldId
    uint32      ZoneId;                                     // 1 m_zoneId
    uint32      NoWarTimeState;                             // 2 m_noWarTimeState
    uint32      WarTimeState;                               // 3 m_warTimeState
    uint32      ukn1;                                       // 4 m_unk1 not known yet, both 900
    uint32      minLevel;                                   // 5 m_minLevel
    uint32      ukn2;                                       // 6 m_unk2
};

// GCC have alternative #pragma pack() syntax and old gcc version not support pack(pop), also any gcc version not support it at some platform
#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

typedef std::set<uint32> SpellCategorySet;
typedef std::map<uint32, SpellCategorySet > SpellCategoryStore;
typedef std::set<uint32> PetFamilySpellsSet;
typedef std::map<uint32, PetFamilySpellsSet > PetFamilySpellsStore;

// Structures not used for casting to loaded DBC data and not required then packing
struct TalentSpellPos
{
    TalentSpellPos() : talent_id(0), rank(0) {}
    TalentSpellPos(uint16 _talent_id, uint8 _rank) : talent_id(_talent_id), rank(_rank) {}

    uint16 talent_id;
    uint8  rank;
};

typedef std::map<uint32, TalentSpellPos> TalentSpellPosMap;

struct SpellEffect
{
    SpellEffect()
    {
        memset(effects, 0, sizeof(effects));
    }
    SpellEffectEntry const* effects[MAX_EFFECT_INDEX];
};

typedef std::map<uint32, SpellEffect> SpellEffectMap;

struct TaxiPathBySourceAndDestination
{
    TaxiPathBySourceAndDestination() : ID(0), price(0) {}
    TaxiPathBySourceAndDestination(uint32 _id, uint32 _price) : ID(_id), price(_price) {}

    uint32    ID;
    uint32    price;
};
typedef std::map<uint32, TaxiPathBySourceAndDestination> TaxiPathSetForSource;
typedef std::map<uint32, TaxiPathSetForSource> TaxiPathSetBySource;

struct TaxiPathNodePtr
{
    TaxiPathNodePtr() : i_ptr(NULL) {}
    TaxiPathNodePtr(TaxiPathNodeEntry const* ptr) : i_ptr(ptr) {}

    TaxiPathNodeEntry const* i_ptr;

    operator TaxiPathNodeEntry const& () const { return *i_ptr; }
};

typedef Path<TaxiPathNodePtr, TaxiPathNodeEntry const> TaxiPathNodeList;
typedef std::vector<TaxiPathNodeList> TaxiPathNodesByPath;

typedef UNORDERED_MAP<uint32 /*frame*/, TransportAnimationEntry const*> TransportAnimationEntryMap;
typedef UNORDERED_MAP<uint32, TransportAnimationEntryMap> TransportAnimationsByEntry;

#define TaxiMaskSize 114
typedef uint8 TaxiMask[TaxiMaskSize];
#endif
