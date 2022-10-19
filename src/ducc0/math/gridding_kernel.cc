/*
 *  This code is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This code is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this code; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* Copyright (C) 2020-2022 Max-Planck-Society
   Author: Martin Reinecke */

#include "ducc0/math/gridding_kernel.h"

namespace ducc0 {

namespace detail_gridding_kernel {

using namespace std;

vector<double> getCoeffs(size_t W, size_t D, const function<double(double)> &func)
  {
  vector<double> coeff(W*(D+1));
  vector<double> chebroot(D+1);
  for (size_t i=0; i<=D; ++i)
    chebroot[i] = cos((2*i+1.)*pi/(2*D+2));
  vector<double> y(D+1), lcf(D+1), C((D+1)*(D+1)), lcf2(D+1);
  for (size_t i=0; i<W; ++i)
    {
    double l = -1+2.*i/double(W);
    double r = -1+2.*(i+1)/double(W);
    // function values at Chebyshev nodes
    double avg = 0;
    for (size_t j=0; j<=D; ++j)
      {
      y[j] = func(chebroot[j]*(r-l)*0.5 + (r+l)*0.5);
      avg += y[j];
      }
    avg/=(D+1);
    for (size_t j=0; j<=D; ++j)
      y[j] -= avg;
    // Chebyshev coefficients
    for (size_t j=0; j<=D; ++j)
      {
      lcf[j] = 0;
      for (size_t k=0; k<=D; ++k)
        lcf[j] += 2./(D+1)*y[k]*cos(j*(2*k+1)*pi/(2*D+2));
      }
    lcf[0] *= 0.5;
    // Polynomial coefficients
    fill(C.begin(), C.end(), 0.);
    C[0] = 1.;
    C[1*(D+1) + 1] = 1.;
    for (size_t j=2; j<=D; ++j)
      {
      C[j*(D+1) + 0] = -C[(j-2)*(D+1) + 0];
      for (size_t k=1; k<=j; ++k)
        C[j*(D+1) + k] = 2*C[(j-1)*(D+1) + k-1] - C[(j-2)*(D+1) + k];
      }
    for (size_t j=0; j<=D; ++j) lcf2[j] = 0;
    for (size_t j=0; j<=D; ++j)
      for (size_t k=0; k<=D; ++k)
        lcf2[k] += C[j*(D+1) + k]*lcf[j];
    lcf2[0] += avg;
    for (size_t j=0; j<=D; ++j)
      coeff[j*W + i] = lcf2[D-j];
    }
  return coeff;
  }

const vector<KernelParams> KernelDB {
{ 4, 1.15,   0.025654879, 1.38734266900695391, 0.543685129620516738, 10.4537368061},
{ 4, 1.20,   0.013809249, 1.30084191659466120, 0.590213748357814927, 8.0543179510},
{ 4, 1.25,  0.0085840685, 1.32740889337637857, 0.595349948691269781, 6.2790471561},
{ 4, 1.30,  0.0057322498, 1.36170633506720939, 0.596563162271272596, 5.0990783187},
{ 4, 1.35,  0.0042494419, 1.38454998763611647, 0.599024129211910394, 4.3546278759},
{ 4, 1.40,  0.0033459552, 1.44053250834175683, 0.592477601614311511, 3.7217220201},
{ 4, 1.45,  0.0028187359, 1.46352200688291756, 0.592944271032464609, 3.3043219539},
{ 4, 1.50,  0.0023843943, 1.55396891642196300, 0.577221731346718170, 2.9381533178},
{ 4, 1.55,  0.0020343796, 1.59910086549113872, 0.572176521407072758, 2.6618577973},
{ 4, 1.60,  0.0017143851, 1.65815463658083817, 0.564474713714742560, 2.4511307281},
{ 4, 1.65,  0.0014730848, 1.71353314121089895, 0.557278858999507465, 2.2748781881},
{ 4, 1.70,  0.0012554492, 1.74643303775221903, 0.554874241511201682, 2.1427042871},
{ 4, 1.75,  0.0010610904, 1.78873269037202931, 0.550987771597484466, 2.0318019872},
{ 4, 1.80, 0.00090885567, 1.81223094321211753, 0.550227397120362727, 1.9390684127},
{ 4, 1.85,  0.0007757401, 1.83044513206242598, 0.550396716048175971, 1.8563046956},
{ 4, 1.90,  0.0006740398, 1.84844873869886173, 0.550237693646838832, 1.7892155532},
{ 4, 1.95, 0.00058655391, 1.87422156840052079, 0.548973894174964228, 1.7252383591},
{ 4, 2.00, 0.00051911189, 1.90694362942565121, 0.546800943467671274, 1.6647289328},
{ 4, 2.05, 0.00047127936, 1.92870295904520117, 0.545942567773495702, 1.6180351024},
{ 4, 2.10, 0.00042991098, 1.94683449766273209, 0.545643124301184002, 1.5750776570},
{ 4, 2.15, 0.00039952939, 1.95983620265510639, 0.545700716364418192, 1.5360335639},
{ 4, 2.20, 0.00036728958, 1.97344450416223927, 0.546036858620087107, 1.5043158068},
{ 4, 2.25, 0.00034355459, 1.98338766723067406, 0.546486536591137662, 1.4750879244},
{ 4, 2.30, 0.00032238422, 1.99225324044062835, 0.547050538525468699, 1.4474775396},
{ 4, 2.35, 0.00030354772, 2.00018570632537562, 0.547697412914564086, 1.4213591181},
{ 4, 2.40, 0.00029003195, 2.00592753653995182, 0.548258342564810208, 1.3969820689},
{ 4, 2.45, 0.00027493243, 2.01243938229898678, 0.548992395112681719, 1.3777706886},
{ 4, 2.50, 0.00026418063, 2.01718769640164686, 0.549588737712608633, 1.3596952708},
{ 5, 1.15,  0.0088036926, 1.42116207957021579, 0.548437022232675320, 20.7590670390},
{ 5, 1.20,  0.0045432118, 1.46045891959780416, 0.550252013660492256, 13.7093230493},
{ 5, 1.25,  0.0025659469, 1.51145374827746592, 0.548237150390439520, 9.8968921178},
{ 5, 1.30,  0.0014949902, 1.56620048447542626, 0.545395964684965207, 7.5461080483},
{ 5, 1.35, 0.00092874124, 1.59406453146680871, 0.546486937476386103, 6.1983298502},
{ 5, 1.40,  0.0005820084, 1.61933118747593419, 0.547748498254474914, 5.1948289082},
{ 5, 1.45, 0.00041837131, 1.67027211745396387, 0.544658443310568785, 4.4063267018},
{ 5, 1.50, 0.00032139657, 1.71066079106240765, 0.543058456183356242, 3.8807455015},
{ 5, 1.55, 0.00025831183, 1.74115262137863080, 0.542447618975519630, 3.4409981145},
{ 5, 1.60, 0.00021156623, 1.76945174343309941, 0.541994323111246512, 3.1280146543},
{ 5, 1.65, 0.00018112326, 1.80697778620925642, 0.539653628674066632, 2.8549504598},
{ 5, 1.70, 0.00015177086, 1.83788206117830111, 0.538217116399792084, 2.6445693919},
{ 5, 1.75, 0.00012345178, 1.88198303872339268, 0.535284977833573650, 2.4671888645},
{ 5, 1.80, 0.00010093043, 1.92251888907660295, 0.532711793419394786, 2.3140796008},
{ 5, 1.85, 8.5743423e-05, 1.97668466305335988, 0.528661095866577169, 2.1719088317},
{ 5, 1.90, 7.5167678e-05, 2.01165901915988377, 0.526719429080505841, 2.0667258527},
{ 5, 1.95, 6.5915521e-05, 2.04017341317655854, 0.525633106330633759, 1.9737859936},
{ 5, 2.00, 5.7747201e-05, 2.06404956765242309, 0.525147614497472048, 1.8905492784},
{ 5, 2.05, 5.1546264e-05, 2.08155391160382619, 0.525099183079613785, 1.8256819603},
{ 5, 2.10, 4.6026099e-05, 2.09675497280340650, 0.525319337441360168, 1.7659898921},
{ 5, 2.15, 4.1922392e-05, 2.10787754202526179, 0.525670765717785704, 1.7118055933},
{ 5, 2.20,  3.755449e-05, 2.11950594134976766, 0.526264418681504975, 1.6680927285},
{ 5, 2.25, 3.4356546e-05, 2.12780076277069163, 0.526887686113864495, 1.6279298967},
{ 5, 2.30, 3.1539667e-05, 2.13487940837656165, 0.527609918351550133, 1.5901784867},
{ 5, 2.35, 2.9093026e-05, 2.14067312365861984, 0.528427270542340510, 1.5546741628},
{ 5, 2.40, 2.7399686e-05, 2.14426220227106956, 0.529153326855788420, 1.5216647807},
{ 5, 2.45, 2.5638396e-05, 2.14865335988313921, 0.530002153989712399, 1.4958089144},
{ 5, 2.50, 2.4438353e-05, 2.15541912455675089, 0.530267776994713835, 1.4709518082},
{ 6, 1.15,  0.0018919684, 1.42845935185068118, 0.545638881017307620, 44.7885835030},
{ 6, 1.20, 0.00087379161, 1.48719490765932405, 0.543418401321749123, 25.6834787342},
{ 6, 1.25, 0.00052387586, 1.55960099215448178, 0.538473314100838540, 16.5953202604},
{ 6, 1.30, 0.00030833805, 1.62939901765606310, 0.533954569665645029, 11.6924548708},
{ 6, 1.35, 0.00018595126, 1.68182947944587102, 0.531554117329473330, 9.0119648310},
{ 6, 1.40, 0.00010913759, 1.71815765571382606, 0.531398241293400742, 7.2252433469},
{ 6, 1.45,  7.446073e-05, 1.77477252124324014, 0.528378935297016072, 5.9079140484},
{ 6, 1.50, 5.3826323e-05, 1.81698002092100541, 0.527149962060523336, 5.0681046522},
{ 6, 1.55, 4.0746477e-05, 1.84778304647709191, 0.526968774374812932, 4.3866613945},
{ 6, 1.60, 3.1441179e-05, 1.87210120783145806, 0.527448880678728860, 3.9189745804},
{ 6, 1.65, 2.6100718e-05, 1.88687453501741254, 0.528237552698065604, 3.5449505448},
{ 6, 1.70, 2.1528068e-05, 1.93427628569917198, 0.525521604287424671, 3.2105281599},
{ 6, 1.75, 1.7177115e-05, 1.97218956889074981, 0.523881015525678118, 2.9593892797},
{ 6, 1.80, 1.3650115e-05, 1.99870075564158722, 0.523237525471655585, 2.7515053554},
{ 6, 1.85, 1.0598995e-05, 2.02197052211952766, 0.522966007931559118, 2.5700389437},
{ 6, 1.90, 8.8157904e-06, 2.06717871852465507, 0.520391356577321385, 2.4117080418},
{ 6, 1.95, 7.6286922e-06, 2.10036738786713473, 0.519019994652832439, 2.2778792946},
{ 6, 2.00, 6.5649967e-06, 2.12725139744555758, 0.518297358983928147, 2.1605777183},
{ 6, 2.05, 5.7476558e-06, 2.14582950527560623, 0.518156358325700173, 2.0707446515},
{ 6, 2.10, 5.0756513e-06, 2.15117834492403848, 0.519528484588510975, 1.9923732045},
{ 6, 2.15, 4.4661935e-06, 2.17431066308956567, 0.518428632279291923, 1.9149544895},
{ 6, 2.20, 3.8877561e-06, 2.18674404559055624, 0.518857199667393298, 1.8558491327},
{ 6, 2.25, 3.4672484e-06, 2.19571772561035061, 0.519317907434135417, 1.8018892421},
{ 6, 2.30, 3.1012426e-06, 2.20337542622801497, 0.519864966538136186, 1.7514811693},
{ 6, 2.35, 2.7894219e-06, 2.20969130786001466, 0.520482025067472431, 1.7043524034},
{ 6, 2.40, 2.5794626e-06, 2.21353127542008288, 0.521029540777087852, 1.6608083693},
{ 6, 2.45, 2.3571404e-06, 2.23178798381241839, 0.520475685196964188, 1.6239332212},
{ 6, 2.50, 2.1615297e-06, 2.25355984648033170, 0.519427413103367708, 1.5885108273},
{ 7, 1.15, 0.00078476028, 1.52487065186644322, 0.528830631677367879, 78.8476415996},
{ 7, 1.20, 0.00027127166, 1.57393487923846775, 0.528799261863398096, 42.6043918351},
{ 7, 1.25, 0.00012594628, 1.62452407223576145, 0.527921777021603034, 26.5645120085},
{ 7, 1.30, 7.0214545e-05, 1.68357459786034158, 0.525748410131269828, 17.8447898947},
{ 7, 1.35, 4.1972457e-05, 1.73434244129960247, 0.523979384430504735, 13.1641437123},
{ 7, 1.40,  2.378019e-05, 1.78450177373615193, 0.522426604532133410, 10.0333416828},
{ 7, 1.45, 1.3863408e-05, 1.81805977900017490, 0.522183476782968392, 8.0807269657},
{ 7, 1.50, 9.1605353e-06, 1.86808227242721681, 0.520627750115673082, 6.7008436434},
{ 7, 1.55,  6.479159e-06, 1.91889800154977697, 0.518313467376310677, 5.5933722048},
{ 7, 1.60, 4.6544571e-06, 1.95361661456243696, 0.517869589070850678, 4.8782531246},
{ 7, 1.65, 3.5489761e-06, 1.97862670804783836, 0.517843025094367637, 4.3197290809},
{ 7, 1.70, 2.7030348e-06, 2.00276665385522978, 0.517857760419206392, 3.8913917347},
{ 7, 1.75, 2.0533894e-06, 2.02899491973198476, 0.517630033611717888, 3.5551103276},
{ 7, 1.80, 1.6069122e-06, 2.05964129442206501, 0.516755193229907150, 3.2595603936},
{ 7, 1.85, 1.2936794e-06, 2.07206068426831935, 0.517874789118745293, 3.0192168398},
{ 7, 1.90, 1.0768664e-06, 2.09089817384387855, 0.518100984742280057, 2.8245214754},
{ 7, 1.95, 9.0890421e-07, 2.10861856953210669, 0.518453784341640800, 2.6508540158},
{ 7, 2.00, 7.7488775e-07, 2.12782841855163740, 0.518637779231286844, 2.4938678816},
{ 7, 2.05, 6.8025539e-07, 2.13005053570979452, 0.520156772624180364, 2.3801651095},
{ 7, 2.10, 6.0222531e-07, 2.13612142484128409, 0.521239720611132551, 2.2732186060},
{ 7, 2.15, 5.0130101e-07, 2.22315454727892003, 0.513773821450151713, 2.1401102226},
{ 7, 2.20, 4.2248762e-07, 2.24084499059128683, 0.513650414956681511, 2.0610125299},
{ 7, 2.25, 3.6494171e-07, 2.25568444545501645, 0.513518089237373299, 1.9890877584},
{ 7, 2.30, 3.1538194e-07, 2.26848817739953335, 0.513579586436226476, 1.9225753980},
{ 7, 2.35, 2.7282924e-07, 2.28153946548291620, 0.513613750771152344, 1.8602736373},
{ 7, 2.40, 2.4350524e-07, 2.29392235445428394, 0.513477734638966399, 1.8025371187},
{ 7, 2.45, 2.1263032e-07, 2.30414895904164929, 0.513820387418989388, 1.7580511161},
{ 7, 2.50, 1.9134836e-07, 2.30764822094622302, 0.514503541751047955, 1.7178450637},
//{ 8, 1.15, 0.00026818611, 1.56812464902853099, 0.522305248110125020, 148.0457899176},
{ 8, 1.20, 7.8028732e-05, 1.62092614496436194, 0.521928717459592240, 72.7476841087},
{ 8, 1.25, 2.7460918e-05, 1.68515851741161127, 0.519925059011194124, 41.3966995640},
{ 8, 1.30, 1.3421658e-05, 1.74423733194385400, 0.518215561816930848, 26.3809527214},
{ 8, 1.35, 7.5158217e-06, 1.78767826435948129, 0.517631950260118323, 18.8432825710},
{ 8, 1.40, 4.2472384e-06, 1.82943219136843593, 0.517186021040191624, 13.9537519919},
{ 8, 1.45, 2.5794802e-06, 1.87169182137764389, 0.516173361086800342, 10.8133434733},
{ 8, 1.50, 1.6131994e-06, 1.92130405416024930, 0.514535088753659164, 8.7539990525},
{ 8, 1.55, 1.0974814e-06, 1.96372291394174914, 0.513400582590337939, 7.1618752078},
{ 8, 1.60,  7.531955e-07, 2.00027613699892992, 0.512884928282042352, 6.1179229589},
{ 8, 1.65, 5.5097346e-07, 2.02756457396499501, 0.512708232352866666, 5.3176877298},
{ 8, 1.70, 4.0136726e-07, 2.04984104058474870, 0.513023766269724568, 4.7233694526},
{ 8, 1.75,  2.906467e-07, 2.07315851719060973, 0.513175715278123445, 4.2650603005},
{ 8, 1.80, 2.1834922e-07, 2.09074187358309427, 0.513604656001378723, 3.8827343851},
{ 8, 1.85, 1.6329905e-07, 2.11645523529999968, 0.513333387771908622, 3.5390042104},
{ 8, 1.90, 1.2828598e-07, 2.12615701543189850, 0.514300442714406669, 3.2905886698},
{ 8, 1.95, 1.0171134e-07, 2.13632066198112369, 0.515235490978862565, 3.0677803052},
{ 8, 2.00, 8.1881369e-08, 2.13970133697098408, 0.516689549706723983, 2.8745397238},
{ 8, 2.05, 6.9121193e-08, 2.14660716984925992, 0.517614537968703048, 2.7204153972},
{ 8, 2.10, 5.9525932e-08, 2.15105265855349437, 0.518691411830401350, 2.5814098526},
{ 8, 2.15, 5.2942463e-08, 2.23657375461814656, 0.512585010403125652, 2.4042224129},
{ 8, 2.20, 4.3612361e-08, 2.26355554888365873, 0.511611490934312019, 2.2972422716},
{ 8, 2.25, 3.6764793e-08, 2.28085130004783121, 0.511282314433412544, 2.2040519855},
{ 8, 2.30, 3.0899101e-08, 2.29611182899012700, 0.511147289892308954, 2.1184496405},
{ 8, 2.35, 2.5951523e-08, 2.30254199712127328, 0.511780483259195784, 2.0422481664},
{ 8, 2.40, 2.2598633e-08, 2.31469675712410483, 0.511707479598551895, 1.9694555537},
{ 8, 2.45, 1.9029665e-08, 2.31862790262141383, 0.512533219200332790, 1.9157619801},
{ 8, 2.50, 1.6752523e-08, 2.33213096803423037, 0.512434861464351243, 1.8621390722},
//{ 9, 1.15, 6.7523935e-05, 1.59279818518454674, 0.518868034497703690, 283.9280715975},
//{ 9, 1.20, 2.2336088e-05, 1.65230682949615670, 0.517781962101589444, 125.1536484833},
{ 9, 1.25, 8.0261034e-06, 1.71038884596711394, 0.516412986082888104, 67.2699421672},
{ 9, 1.30, 3.2272675e-06, 1.77686383492239219, 0.514182130169785867, 39.8978741509},
{ 9, 1.35, 1.6398132e-06, 1.82592737318345066, 0.513193942844713979, 27.0998903831},
{ 9, 1.40, 8.5542435e-07, 1.87067759357321761, 0.512633239868663493, 19.2674029746},
{ 9, 1.45, 4.8998062e-07, 1.90795621790997383, 0.512263097727883476, 14.5448110073},
{ 9, 1.50, 2.8357238e-07, 1.93762657349720735, 0.512746071669525239, 11.6639453501},
{ 9, 1.55, 1.7632448e-07, 1.98311308916759410, 0.511427659541475843, 9.2653677785},
{ 9, 1.60, 1.1241387e-07, 2.00310475055321913, 0.512693234528368458, 7.8330467363},
{ 9, 1.65, 8.0252028e-08, 2.02853832695709624, 0.512772600687914748, 6.6826892762},
{ 9, 1.70,  5.741767e-08, 2.05749103416742773, 0.512442682820275941, 5.8173115352},
{ 9, 1.75, 4.0256578e-08, 2.08951740049157353, 0.511769319168294379, 5.1579075720},
{ 9, 1.80, 2.8882533e-08, 2.12569139594204293, 0.510474430030262272, 4.5984436748},
{ 9, 1.85, 2.0673982e-08, 2.14084796481061401, 0.511166573108337241, 4.1620237817},
{ 9, 1.90, 1.5495159e-08, 2.15220897725283811, 0.511920537971019773, 3.8317647164},
{ 9, 1.95, 1.1746262e-08, 2.16302589188831629, 0.512710692987980332, 3.5401148496},
{ 9, 2.00, 9.1018889e-09, 2.17056767744863377, 0.513731733799825729, 3.2851053736},
{ 9, 2.05,  7.497767e-09, 2.18157780989507843, 0.514270825921971819, 3.0832842567},
{ 9, 2.10, 6.3513381e-09, 2.19378396150069488, 0.514701597294465074, 2.8993754235},
{ 9, 2.15, 5.5877468e-09, 2.22555473166315076, 0.513362012714964733, 2.7177809671},
{ 9, 2.20, 4.5551165e-09, 2.27462456520236911, 0.510642927649489375, 2.5645862960},
{ 9, 2.25, 3.7610362e-09, 2.29533513392633504, 0.510050300146584146,   2.4449056380},
{ 9, 2.30, 3.0828204e-09, 2.31163401238828214, 0.509842919945291184,   2.3371090420},
{ 9, 2.35, 2.5194491e-09, 2.32062596844457891, 0.510221460581078756,   2.2410920736},
{ 9, 2.40, 2.1430663e-09, 2.33096981577762863, 0.510299156316853630,   2.1517975272},
{ 9, 2.45,  1.758345e-09, 2.33944039939814630, 0.510740318181346686,   2.0837106381},
{ 9, 2.50, 1.5226889e-09, 2.35149174534770866, 0.510761951134299674,   2.0184986773},
//{10, 1.15, 2.3448628e-05, 1.62091906093269023, 0.514982625064470012, 531.7304435466},
//{10, 1.20, 5.9153174e-06, 1.69234201235989556, 0.513094798212222258, 206.6555526926},
//{10, 1.25, 2.0151277e-06, 1.74786752491038144, 0.512265223057913333, 104.9031607572},
{10, 1.30, 7.6037409e-07, 1.80372732132093017, 0.511193804218137204,  60.1240814578},
{10, 1.35, 3.5081762e-07, 1.83716023474779733, 0.512031562295730325,  39.9625867287},
{10, 1.40, 1.7691912e-07, 1.85391509845475611, 0.514418283311950941,  28.2278204930},
{10, 1.45, 9.5898586e-08, 1.90830769839072301, 0.512379641402013419,  20.1275205787},
{10, 1.50, 5.1649487e-08, 1.95494826274085320, 0.511078734295393411,  15.4621270544},
{10, 1.55, 2.9344166e-08, 1.99350394992425262, 0.510397724633050442,  12.0338227824},
{10, 1.60, 1.6984065e-08, 2.01342359582482144, 0.511475165089376849,   9.9815674059},
{10, 1.65, 1.1201377e-08, 2.02782788433943129, 0.512469958472225984,   8.4241715610},
{10, 1.70,  7.739247e-09, 2.05508887158668019, 0.512389656778032943,   7.2129166360},
{10, 1.75, 5.4226206e-09, 2.09842561348808276, 0.510859505739114650,   6.2556726833},
{10, 1.80,  3.805106e-09, 2.13026501956985026, 0.510023956437886783,   5.5128602887},
{10, 1.85, 2.6039482e-09, 2.15851858754070269, 0.509535290304133692,   4.8998938688},
{10, 1.90, 1.8492236e-09, 2.17074456401322502, 0.510142953610635330,   4.4675507143},
{10, 1.95, 1.3147032e-09, 2.18179820090263776, 0.510828472025324998,   4.0906487981},
{10, 2.00, 9.6449672e-10, 2.20185827419722724, 0.510932926955372735,   3.7438836821},
{10, 2.05, 7.6846768e-10, 2.21550763306063159, 0.511269444796564776,   3.4860441128},
{10, 2.10, 6.2483494e-10, 2.22412325164730218, 0.511929317514754234,   3.2608023989},
{10, 2.15, 5.3808333e-10, 2.22816157600828868, 0.512646623240830435,   3.0637222785},
{10, 2.20, 4.5009578e-10, 2.27922379461907365, 0.510102647371714735,   2.8672018065},
{10, 2.25, 3.6482161e-10, 2.30330743716074871, 0.509303685953674257,   2.7149916841},
{10, 2.30, 2.9127779e-10, 2.31984455307209547, 0.509102194176802358,   2.5812582846},
{10, 2.35, 2.3142805e-10, 2.33087221488618024, 0.509321314291704130,   2.4617690105},
{10, 2.40, 1.9431938e-10, 2.34624101281160602, 0.509078430399858783,   2.3494659493},
{10, 2.45, 1.6154762e-10, 2.36320011400279606, 0.508957408759996954,   2.2621419175},
{10, 2.50, 1.3946655e-10, 2.37159802874616110, 0.509220274554885499,   2.1853783852},
//{11, 1.15, 7.9162516e-06, 1.64728362199167599, 0.511934334804218016, 974.6430811638},
//{11, 1.20, 1.7270227e-06, 1.71570166968587401, 0.510468090249409534, 350.0847862159},
//{11, 1.25,  5.331949e-07, 1.77233328700735848, 0.509675372203101662, 165.8133106104},
{11, 1.30, 1.8775393e-07, 1.82059946634911629, 0.509450238102371999,  91.3201175932},
{11, 1.35, 7.7356018e-08, 1.85182149538065155, 0.510370245526541200,  58.5030705785},
{11, 1.40, 3.2528372e-08, 1.88805128164528946, 0.510804839340288086,  38.7947410857},
{11, 1.45, 1.6597763e-08, 1.91654714810772142, 0.511384035585156882,  27.6492222899},
{11, 1.50, 8.8600676e-09, 1.96250712307515829, 0.510315625273379592,  20.6436674983},
{11, 1.55, 4.8306855e-09, 2.00491987445132214, 0.509302085240925018,  15.5849970139},
{11, 1.60, 2.5766954e-09, 2.02953716980096699, 0.509885445056624920,  12.6301961756},
{11, 1.65, 1.5599028e-09, 2.04444864076712296, 0.510765361413245800,  10.4746925185},
{11, 1.70,   9.92589e-10, 2.06275271973441621, 0.511381096553326309,   8.8874091754},
{11, 1.75, 6.9033076e-10, 2.08812626177778693, 0.511447196802164883,   7.6797225266},
{11, 1.80, 4.9444644e-10, 2.13841582017241327, 0.509284628319329635,   6.5919278532},
{11, 1.85, 3.3291892e-10, 2.17142864368980160, 0.508438028450355906,   5.7712386857},
{11, 1.90, 2.3006774e-10, 2.18891031764846167, 0.508617441707654327,   5.1982477880},
{11, 1.95, 1.5880459e-10, 2.20134649616651634, 0.509162724748768891,   4.7146996707},
{11, 2.00, 1.1177181e-10, 2.21475117291316304, 0.509642111661072761,   4.2910236853},
{11, 2.05, 8.5919058e-11, 2.23040455079536271, 0.509802106041026382,   3.9629787132},
{11, 2.10, 6.8731617e-11, 2.24100621957522605, 0.510240936233531728,   3.6792458935},
{11, 2.15, 5.8344602e-11, 2.26191099329236911, 0.509806981838504036,   3.4127228343},
{11, 2.20,  4.567188e-11, 2.30073992321575593, 0.508264261153209440,   3.1868617285},
{11, 2.25, 3.5832111e-11, 2.31926534799677198, 0.507931437041867717,   3.0062980336},
{11, 2.30, 2.7608185e-11, 2.33309438898428834, 0.507941054797856339,   2.8457625350},
{11, 2.35, 2.1403798e-11, 2.35034864074681993, 0.507774983071945241,   2.6957501461},
{11, 2.40, 1.8356566e-11, 2.36340048357702459, 0.507735577211737898,   2.5621440688},
{11, 2.45, 1.4850721e-11, 2.38071638477489644, 0.507574226025855668,   2.4573165244},
{11, 2.50, 1.2565305e-11, 2.38811504113443407, 0.507880153614157170,   2.3662973169},
//{12, 1.15, 2.7535895e-06, 1.66618375292687060, 0.509817214578047961, 1807.6530127615},
//{12, 1.20, 5.2570039e-07, 1.72945574696824744, 0.508923959486823119, 603.3245415625},
//{12, 1.25,  1.378658e-07, 1.76981823809111005, 0.509924071828923520, 277.0531195739},
//{12, 1.30, 4.4329166e-08, 1.80920424348691644, 0.510607427082967802, 145.9984267338},
{12, 1.35, 1.7038989e-08, 1.86191125960623949, 0.509383233748238351,  85.7993986110},
{12, 1.40, 6.5438745e-09, 1.90691474853212251, 0.508947988827302411,  54.0230437349},
{12, 1.45, 2.9874755e-09, 1.93183980799754740, 0.509808232527001470,  37.5434593707},
{12, 1.50, 1.4920453e-09, 1.96284831611742594, 0.510098575176861413,  27.7810027675},
{12, 1.55, 8.0989259e-10, 2.01298478059999963, 0.508532780498326398,  20.2120120313},
{12, 1.60, 4.1660516e-10, 2.05179217431937877, 0.507910239832440991,  15.8407353350},
{12, 1.65, 2.3539714e-10, 2.06983884002812291, 0.508513106389275826,  12.8869464271},
{12, 1.70,  1.349729e-10, 2.08873653557463435, 0.509041714629797859,  10.7753325440},
{12, 1.75,   8.32567e-11, 2.10695573323961627, 0.509592067203735621,   9.2458646971},
{12, 1.80, 5.8834574e-11, 2.13594152228332446, 0.509188706919260214,   7.9377448346},
{12, 1.90, 2.6412844e-11, 2.20063695058204400, 0.507588969895806597,   6.0605051280},
{12, 1.95, 1.7189519e-11, 2.21467416262245198, 0.508001740435055416,   5.4424849014},
{12, 2.00, 1.2174676e-11, 2.24313922013542122, 0.507519117668178810,   4.8722260313},
{12, 2.05, 9.1796527e-12, 2.25800050476701308, 0.507725360078557952,   4.4707949449},
{12, 2.10, 6.9968187e-12, 2.26768383023200615, 0.508244362603822442,   4.1254387502},
{12, 2.15, 5.7593121e-12, 2.27419945142848912, 0.508711821402158537,   3.8250707710},
{12, 2.20, 4.4531108e-12, 2.31215083139197430, 0.507321166311744398,   3.5498207672},
{12, 2.25,  3.409228e-12, 2.32915976957524506, 0.507120993515833507,   3.3321539874},
{12, 2.30, 2.5695873e-12, 2.34019945326078860, 0.507315647115769375,   3.1410169081},
{12, 2.35, 2.0819171e-12, 2.35839864071708671, 0.507164758327537046,   2.9588603186},
{12, 2.40, 1.7482127e-12, 2.37580916951412302, 0.506811755804958630,   2.7952855777},
{12, 2.45, 1.3958182e-12, 2.38543795364483735, 0.507151734357849882,   2.6764185502},
{12, 2.50, 1.1607148e-12, 2.39308673699843633, 0.507384592903633247,   2.5680946742},
//{13, 1.15, 9.2170382e-07, 1.68363011637777182, 0.507947579733076249, 3328.1348981716},
//{13, 1.20, 1.6209681e-07, 1.73480771453549720, 0.508431864244146658, 1058.0272189253},
//{13, 1.25, 3.7397431e-08, 1.79648284490154042, 0.507197222045779328, 428.8429195213},
//{13, 1.30, 1.0186843e-08, 1.82834385283036238, 0.508646797832505881, 219.0916545806},
//{13, 1.35, 3.6754593e-09, 1.88109727197091137, 0.507543935224009091, 123.2872678579},
{13, 1.40, 1.3183305e-09, 1.91910937296080086, 0.507746842923307073,  75.6932813509},
{13, 1.45, 5.3619018e-10, 1.94707297150615211, 0.508282466571504332,  50.8065375864},
{13, 1.50, 2.2322814e-10, 1.97398687775712633, 0.508895915810955501,  36.8766620559},
{13, 1.55, 1.1526196e-10, 2.00332592754192129, 0.509086660244242095,  26.7550297918},
{13, 1.60,  6.142585e-11, 2.05109897211710734, 0.507856387619474159,  20.2729866655},
{13, 1.65, 3.4051523e-11, 2.08185279122843703, 0.507477582870768273,  15.9899455555},
{13, 1.70, 1.8583915e-11, 2.10456439241444127, 0.507720495999597454,  13.1250459104},
{13, 1.75, 1.0809637e-11, 2.12432719081891719, 0.508163399728273291,  11.1060802908},
{13, 1.80, 7.3773949e-12, 2.14677563668683868, 0.508199624642145742,   9.4647755975},
{13, 1.85, 4.9615192e-12, 2.18514838218796648, 0.507138335236436033,   8.0411215048},
{13, 1.90, 3.2631088e-12, 2.20863767182662052, 0.506876395917062239,   7.0737254925},
{13, 1.95, 2.0843845e-12, 2.22992405512335834, 0.506802366180825303,   6.2667609761},
{13, 2.00, 1.3665346e-12, 2.24918412689287717, 0.506897603636508043,   5.5877456941},
{13, 2.05, 1.0047051e-12, 2.26665189872822648, 0.506920204081998560,   5.0825521005},
{13, 2.10, 7.5179108e-13, 2.22048979304976779, 0.511964017087828127,   4.7762420255},
{13, 2.15, 6.0368602e-13, 2.29661672025243302, 0.506990606475567596,   4.2641813232},
{13, 2.20,  4.381128e-13, 2.32427662672050284, 0.506327270795296824,   3.9503965751},
{13, 2.25, 3.1904543e-13, 2.33844990065624136, 0.506321797962568043,   3.6924744900},
{13, 2.30, 2.4813399e-13, 2.34653865082254631, 0.506767097465848138,   3.4664540395},
{13, 2.35, 1.9677242e-13, 2.34597852968911846, 0.507790111945015443,   3.2703984095},
{13, 2.40, 1.5731562e-13, 2.33876301185372659, 0.509304264877679325,   3.0968887696},
{13, 2.45,  1.260707e-13, 2.33722057964387409, 0.511038136583125358,   2.9587690352},
{13, 2.50, 1.0381402e-13, 2.40848762399950456, 0.506240892127974074,   2.7779279748},
//{14, 1.15, 3.3179964e-07, 1.69037955464157319, 0.507213798472346600, 6364.6108510969},
//{14, 1.20, 4.7410641e-08, 1.73605649536925855, 0.508227276777077464, 1882.8049204091},
//{14, 1.25, 9.7512138e-09, 1.79691927457418443, 0.507147774670714369, 707.4299973596},
//{14, 1.30, 2.3054396e-09, 1.83411586422257855, 0.507963691016771302, 337.6157045698},
//{14, 1.35, 6.9020252e-10, 1.86751486350487861, 0.508559413885167899, 189.0913692122},
{14, 1.45, 1.0233695e-10, 1.95775461107537807, 0.507318501523569942,  68.9325178636},
{14, 1.50, 4.0217472e-11, 1.99085138976692000, 0.507420679004011443,  48.3726629993},
{14, 1.55, 1.8309682e-11, 2.01994550000776218, 0.507627981831691222,  34.2820919546},
{14, 1.60, 9.3253297e-12, 2.05646872686597915, 0.507273278839521558,  25.7812291027},
{14, 1.65, 5.0814261e-12, 2.09109786917960516, 0.506639100660506880,  19.8658065064},
{14, 1.70, 2.6117534e-12, 2.11690920750357670, 0.506645796545879490,  16.0095721964},
{14, 1.75, 1.3836173e-12, 2.14296497619833071, 0.506688333430271420,  13.2964030699},
{14, 1.80, 8.4404451e-13, 2.10589494510534747, 0.512009584794536954,  11.7061090822},
{14, 1.85, 5.6077817e-13, 2.18483474049175896, 0.506939183982713537,   9.5355293293},
{14, 1.90, 3.6476541e-13, 2.21441009886636486, 0.506339854133940448,   8.2624552263},
{14, 1.95, 2.2821308e-13, 2.23872059994321404, 0.506104469798136170,   7.2344561831},
{14, 2.00, 1.5663258e-13, 2.26620333981011957, 0.505702727284752185,   6.3610202495},
{14, 2.05, 1.1163287e-13, 2.22921962953765984, 0.509730225677966442,   5.9151137994},
{14, 2.10, 7.9859271e-14, 2.29375063089635178, 0.506159374712625398,   5.2303085756},
{14, 2.15, 6.2381259e-14, 2.30623578873349011, 0.506233284885936219,   4.7747138735},
{14, 2.20,  4.356129e-14, 2.33516338286240632, 0.505529489820179534,   4.3941548697},
{14, 2.25, 3.1404202e-14, 2.34534092482061407, 0.505760456148805271,   4.0930163493},
{14, 2.30, 2.4175153e-14, 2.35351104464167582, 0.506190553983114300,   3.8233047060},
{14, 2.35, 1.8357712e-14, 2.35215813163294785, 0.507248896374602132,   3.5916967756},
{14, 2.40, 1.4099974e-14, 2.34888026011766415, 0.508519429090298569,   3.3814606938},
{14, 2.45, 1.0935617e-14, 2.34867076167144040, 0.510003304669525259,   3.2187532783},
{14, 2.50, 9.5788106e-15, 2.35367036387341244, 0.510685840514810585,   3.0652393700},
//{15, 1.15, 1.1497825e-07, 1.69424027220171936, 0.506831582159718241, 12272.4679256358},
//{15, 1.20, 1.3576122e-08, 1.75126605923500911, 0.506715729511454271, 3165.8331024102},
//{15, 1.25, 2.4191274e-09, 1.78940738610350270, 0.507752220645187435, 1199.2566298938},
//{15, 1.30, 5.6425811e-10, 1.84333326920019647, 0.507077616979145995, 513.6786702358},
//{15, 1.35, 1.5385589e-10, 1.88662475733681179, 0.506844959030130493, 269.5800241006},
//{15, 1.40, 4.5268476e-11, 1.92077955244841569, 0.507329460173776337, 153.9821983083},
//{15, 1.45, 1.8385566e-11, 1.90159637709349205, 0.512515137074920912, 104.5784450345},
{15, 1.50, 6.7744371e-12, 2.00313509049261684, 0.506369126860134799,  63.6692118131},
{15, 1.55, 2.6624475e-12, 2.02769096512803149, 0.506859221697922968,  44.3482064659},
{15, 1.60, 1.2491768e-12, 2.05942756017910877, 0.506895351489686119,  32.8560158667},
{15, 1.65, 7.1369746e-13, 2.09589892460849780, 0.506199444836538670,  24.7621791224},
{15, 1.70, 3.6730718e-13, 2.12561308470312227, 0.505936448795062499,  19.5591924043},
{15, 1.75, 1.8617044e-13, 2.14978468419627511, 0.506064959463464525,  16.0612777106},
{15, 1.80, 1.1401731e-13, 2.13070355160834035, 0.509147033300853535,  13.8494715655},
{15, 1.85,  7.025497e-14, 2.14640454430278194, 0.510157770870503957,  11.6254883700},
{15, 1.90, 4.3931095e-14, 2.22222581651304507, 0.505670515772177032,   9.6315126951},
{15, 1.95, 2.6366629e-14, 2.24191498378182752, 0.505700537363949709,   8.3802170960},
{15, 2.00, 1.6870067e-14, 2.23905288989111373, 0.507224857972450427,   7.4431961171},
{15, 2.05, 1.0906518e-14, 2.24788726470938416, 0.508304072687188468,   6.6855908943},
{15, 2.10, 8.5300392e-15, 2.27425538184085552, 0.507519859910458782,   5.9833167178},
{15, 2.15, 6.5967269e-15, 2.28245791310618795, 0.508045394647576032,   5.4302828285},
{15, 2.20,  4.844036e-15, 2.34304771216252306, 0.504920027159622142,   4.8916907244},
{15, 2.25, 3.8258573e-15, 2.34910087874066820, 0.505413263940583724,   4.5416073131},
{15, 2.30, 3.1888673e-15, 2.34756334672210221, 0.506264170181648487,   4.2411476755},
{15, 2.35, 2.8370507e-15, 2.36055774630180348, 0.506081914024954727,   3.9448532210},
{15, 2.40, 2.4548132e-15, 2.36490847696750528, 0.507615615591247793,   3.6795502862},
{15, 2.45, 2.2750614e-15, 2.35051940865287179, 0.509950770552300314,   3.5095437990},
{15, 2.50, 2.1972316e-15, 2.40806330453155271, 0.505868719498627328,   3.2794280733},
{16, 1.50, 1.2096628e-12, 2.01307877027065762, 0.505558796420728096,  83.8820321354},
{16, 1.55, 4.5908721e-13, 2.04380326110740373, 0.505630968200927344,  56.5038682093},
{16, 1.60, 1.7688124e-13, 2.03295618226724573, 0.508904567193512403,  43.5164017130},
{16, 1.65, 9.0069565e-14, 2.04945147346240075, 0.510358260451508228,  32.6663514915},
{16, 1.70, 5.8034554e-14, 2.09251197835556058, 0.508376740134453664,  24.9843669979},
{16, 1.75, 2.7184299e-14, 2.14615240307016419, 0.506203783339794433,  19.5895747335},
{16, 1.80, 1.6016321e-14, 2.14900401818201470, 0.508272182908355852,  16.3148441565},
{16, 1.85, 1.1839029e-14, 2.18118268121603531, 0.507257005955753293,  13.4170049762},
{16, 1.90, 8.8632771e-15, 2.19811765787113389, 0.507584087084441804,  11.4851061764},
{16, 1.95, 6.2455091e-15, 2.23400113434959913, 0.506013310377748882,   9.7864585611},
{16, 2.00, 4.7004841e-15, 2.26216319093441998, 0.505692467527165790,   8.4269547607},
{16, 2.05, 4.2646986e-15, 2.25329083408322051, 0.507567253881010094,   7.6171657731},
{16, 2.10, 3.9253089e-15, 2.27303066365145101, 0.507594989653377460,   6.7828123291},
{16, 2.15, 3.4365722e-15, 2.22871157639923112, 0.512568481278005583,   6.2818488913},
{16, 2.20, 3.1908649e-15, 2.26209075670319004, 0.511764085681784886,   5.6742302870},
{16, 2.25, 2.8626918e-15, 2.34001344001939637, 0.505337849348090895,   5.0791132067},
{16, 2.30, 2.7071235e-15, 2.22162396743223489, 0.514357453143778454,   4.9923394278},
{16, 2.35, 2.3926771e-15, 2.32224111840336445, 0.508718765504795778,   4.4128099480},
{16, 2.40, 2.2145405e-15, 2.27405543163524015, 0.516651203724702857,   4.1544277860},
{16, 2.45,  2.061095e-15, 2.31372845718823461, 0.505523154280454690,   3.9577046439},
{16, 2.50, 2.2824172e-15, 2.24437054609444520, 0.513575525543478850,   3.8284360223}};

template<typename T> T esknew (T v, T beta, T e0)
  {
  auto tmp = (1-v)*(1+v);
  auto tmp2 = tmp>=0;
  return tmp2*exp(beta*(pow(tmp*tmp2, e0)-1));
  }

shared_ptr<PolynomialKernel> selectKernel(size_t idx)
  {
  MR_assert(idx<KernelDB.size(), "no appropriate kernel found");
  auto supp = KernelDB[idx].W;
  auto beta = KernelDB[idx].beta*supp;
  auto e0 = KernelDB[idx].e0;
  auto lam = [beta,e0](double v){return esknew(v, beta, e0);};
  return make_shared<PolynomialKernel>(supp, supp+3, lam, GLFullCorrection(supp, lam));
  }

}}
