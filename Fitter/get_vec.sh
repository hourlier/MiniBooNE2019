#!/bin/sh

rm null_chi2_200.vec
rm null_chi2_475.vec
rm bf_chi2_200.vec
rm bf_chi2_475.vec


grep "CHI2 Null" null_prob_11.27_200.out | cut -d"=" -f2 > null_chi2_200.vec
grep "CHI2 Null" null_prob_11.27_475.out | cut -d"=" -f2 > null_chi2_475.vec
grep "CHI2 BF" bf_prob_11.27_200.out | cut -d"=" -f2 > bf_chi2_200.vec
grep "CHI2 BF" bf_prob_11.27_475.out | cut -d"=" -f2 > bf_chi2_475.vec
