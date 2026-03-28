#include "GameConfig.h"

GameConfig::GameConfig(int l_i, int w, int h, 
                      int hp, int d, int rd,
                      int l, int s,
                      int ec, int ehp,
                      int ed, int oc,
                      int ohp, int od,
                      int ocr, int vc,
                      int bd, int bl,
                      int bs, int rr,
                      int sc, int jsd,
                      int jsr, int jsc,
                      int msd, int msr,
                      int msc, int fnd,
                      int fnc, int fnr) 
    : lvl_index(l_i), sea_width(w), sea_height(h),
      health(hp), common_damage(d), ranged_damage(rd),
      ranged_range(rr), pl_level(l), pl_score(s),
      pl_start_pos(0, 0), en_count(ec),
      en_hp(ehp), en_damage(ed),
      oct_count(oc), oct_hp(ohp),
      oct_damage(od), oct_range(ocr), vortex_count(vc),
      boost_damage(bd), boost_level(bl),
      boost_score(bs), spells_count(sc),
      jet_spell_damage(jsd), jet_spell_range(jsr),
      jet_spell_cost(jsc), mine_spell_damage(msd),
      mine_spell_range(msr), mine_spell_cost(msc),
      fish_net_damage(fnd), fish_net_cost(fnc),
      fish_net_range(fnr){}

void GameConfig::saveTo(SaveFileRAII& out) const {
  out.write(lvl_index).write(sea_width).write(sea_height).write(health)
    .write(common_damage).write(ranged_damage).write(ranged_range)
    .write(pl_level).write(pl_score)
    .write(pl_start_pos)
    .write(en_count).write(en_hp).write(en_damage)
    .write(oct_count).write(oct_hp).write(oct_damage).write(oct_range)
    .write(vortex_count)
    .write(boost_damage).write(boost_level).write(boost_score)
    .write(spells_count)
    .write(jet_spell_damage).write(jet_spell_range).write(jet_spell_cost)
    .write(mine_spell_damage).write(mine_spell_range).write(mine_spell_cost)
    .write(fish_net_damage).write(fish_net_cost).write(fish_net_range);
  }

void GameConfig::loadFrom(SaveFileRAII& in){
  in.read(lvl_index).read(sea_width).read(sea_height).read(health)
    .read(common_damage).read(ranged_damage).read(ranged_range)
    .read(pl_level).read(pl_score)
    .read(pl_start_pos)
    .read(en_count).read(en_hp).read(en_damage)
    .read(oct_count).read(oct_hp).read(oct_damage).read(oct_range)
    .read(vortex_count)
    .read(boost_damage).read(boost_level).read(boost_score)
    .read(spells_count)
    .read(jet_spell_damage).read(jet_spell_range).read(jet_spell_cost)
    .read(mine_spell_damage).read(mine_spell_range).read(mine_spell_cost)
    .read(fish_net_damage).read(fish_net_cost).read(fish_net_range);
}