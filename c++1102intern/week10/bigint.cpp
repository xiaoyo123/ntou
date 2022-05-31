/*
Author: Xiaoyo
num: 01057020
time: 2022 / 05 / 15
//*/
#include "iostream"
#include "bigint.h"
#include "malloc.h"
#include "string.h"
#include "algorithm"
#include <fstream>
using namespace std;

void ans(char digit[], int& numdigit, int &cap, int& sign1, char num[], int& n, int& sign2) {
	int* ans;
	ans = new int[max(numdigit, n) + 10];
	for (int i = 0; i < max(numdigit, n) + 10; i++) ans[i] = 0;

	if (sign1 == sign2) {
		int carry = 0, i = 0;
		for (; i < numdigit && i < n; ++i) {
			ans[i] = (digit[i] + num[i] + carry) - 96;
			ans[i] / 10 ? ans[i] %= 10, carry = 1 : carry = 0;
		}
		while (i < numdigit) {
			ans[i] = (digit[i] + carry) - 48;
			ans[i] / 10 ? ans[i] %= 10, carry = 1 : carry = 0;
			++i;
		}
		while (i < n) {
			ans[i] = (num[i] + carry) - 48;
			ans[i] / 10 ? ans[i] %= 10, carry = 1 : carry = 0;
			++i;
		}
		if (carry) ans[i++] += 1;
		numdigit = i;
		cap = numdigit + 10;
		for (int j = 0; j < i; j++) digit[j] = ans[j] + 48;
	}
	else {
		char* num1, *num2;
		num1 = new char[max(cap, n)]; num2 = new char[max(cap, n)];
		while (digit[numdigit - 1] == '0') numdigit--;
		while (num[n - 1] == '0') n--;

		for (int i = 0; i < numdigit; i++) num1[i] = digit[i];
		for (int i = 0; i < n; i++) num2[i] = num[i];

		if (numdigit < n) {
			swap(num1, num2);
			swap(numdigit, n);
			swap(sign1, sign2);
		}
		else if (numdigit == n) {
			for (int i = numdigit - 1; i >= 0; i--) {
				if (num1[i] < num2[i]) {
					swap(num1, num2);
					swap(sign1, sign2);
					break;
				}
			}
		}
		sign1 = sign1 < 0 ? -1 : 1;
		int i = 0, borrow = 0;
		for (; i < n; i++) {
			ans[i] = num1[i] - num2[i] - borrow;
			ans[i] < 0 ? ans[i] += 10, borrow = 1 : borrow = 0;
		}
		while (borrow) {
			ans[i] = num1[i] - borrow - 48;
			ans[i] < 0 ? ans[i] += 10, borrow = 1 : borrow = 0;
			i++;
		}
		while (i < numdigit) {
			ans[i] = num1[i] - '0';
			i++;
		}
		for (int j = 0; j < i; j++) {
			digit[j] = ans[j] + 48;
		}
	}
	free(ans);
}

bigint::bigint() {
	cap = 100;
	digit = new char[cap + 10];
	zero();
}
bigint::bigint(int v, int size = 100) {
	digit = 0; cap = numdigit = 0;

	int temp = v, sz = 0;
	while (temp) {
		temp /= 10;
		sz++;
	}
	if (sz > size) size = sz;
	cap = size;
	digit = new char[cap + 10];
	zero();

	v < 0 ? sign = -1, v = -v : sign = 1;

	numdigit = sz; int n = 0;
	while (v) {
		digit[n] = v % 10 + 48;
		v /= 10; n++;
	}
}
bigint::bigint(const char* const s, int size = 100) {
	digit = 0; cap = numdigit = 0;

	int sz = strlen(s), i;
	sz = s[0] == '-' ? sz - 1 : sz;
	sz = s[0] == '+' ? sz - 1 : sz;
	if (sz > size) size = sz;
	cap = size;
	digit = new char[cap + 10];
	zero();

	if (s[0] == '+' || s[0] == '-')
		i = 1, sign = (s[0] == '+' ? 1 : -1);
	else i = 0;

	numdigit = sz;
	for (int k = sz - 1; i <= sz; i++, k--) {
		if (s[i] < '0' || s[i] > '9') continue;
		digit[k] = s[i];
	}
}
bigint::bigint(bigint& x) {
	cap = x.cap; numdigit = x.numdigit; sign = x.sign;
	digit = new char[cap];
	for (int i = 0; i < cap; i++) digit[i] = x.digit[i];
}
bigint::~bigint() {
	if (cap) {
		if (cap == 1) delete digit;
		else         delete[] digit;
	}
}
bigint::bigint(char* n, int x, int y, int z) {
	numdigit = x, cap = y, sign = z;
	digit = new char[cap];
	for (int i = 0; i < cap; i++) digit[i] = n[i];
}

void bigint::zero() {
	if (cap) {
		numdigit = 1;
		digit[0] = '0';
		sign = 1;
	}
}
void bigint::printvalue(ostream &out) {
	while (digit[numdigit - 1] == '0') numdigit--;
	if (!numdigit) out << 0;
	else {
		cout << (sign == -1 ? "-" : "");
		for (int i = numdigit - 1; i >= 0; i--) out << digit[i];
	}
	return;
}
void bigint::update(int v) {
	sign = v < 0 ? -1 : 1;
	v = abs(v);
	int n = 0;
	while (v) {
		digit[n] = v % 10 + 48;
		v /= 10; n++;
	}
	numdigit = n;
}
void bigint::update(bigint x) {
	cap = x.cap; numdigit = x.numdigit; sign = x.sign;
	digit = new char[cap];
	for (int i = 0; i < cap; i++) digit[i] = x.digit[i];
}

ostream& operator << (ostream& out, bigint& x) {
	x.printvalue(out);
	return out;
}
bigint bigint::operator = (int v) {
	update(v);
	bigint y(digit, numdigit, cap, sign);
	return y;
}
bigint bigint::operator = (bigint x) {
	update(x);
	bigint y(digit, numdigit, cap, sign);
	return y;
}
bigint bigint::operator += (bigint x) {
	ans(digit, numdigit, cap, sign, x.digit, x.numdigit, x.sign);
	bigint y(digit, numdigit, cap, sign);
	return y;
}
bigint bigint::operator += (int x) {
	bigint a(x);
	ans(digit, numdigit, cap, sign, a.digit, a.numdigit, a.sign);
	bigint y(digit, numdigit, cap, sign);
	return y;
}
bigint bigint::operator + (bigint x) {
	ans(digit, numdigit, cap, sign, x.digit, numdigit, x.sign);
	bigint y(digit, numdigit, cap, sign);
	return y;
}
bigint bigint::operator + (int x) {
	bigint a(x);
	ans(digit, numdigit, cap, sign, a.digit, a.numdigit, a.sign);
	bigint y(digit, numdigit, cap, sign);
	return y;
}
bigint bigint::operator ++ () {
	bigint a(1);
	ans(digit, numdigit, cap, sign, a.digit, a.numdigit, a.sign);
	bigint y(digit, numdigit, cap, sign);
	return y;
}
bigint bigint::operator ++ (int x) {
	bigint a(*this);
	bigint b(1);
	ans(digit, numdigit, cap, sign, b.digit, b.numdigit, b.sign);
	return a;
}

void test() {
	bigint a("97588001472304413649400677376297242677894951334379021852314406871098468506361443800998734636383151489917282981220333563545179214127687907408658681792783759354630483925194692026229150454814126712419874210061553966701344909993797030296751745895482073438416508699818513251962414440472502341255995981032399985859950034352819998393359401760718242479629613894970893273274977254299861866685434677198949089426853904463200230306444948887927343358498989185386856884540537679089762967533791654771195778258971312674268732486874972379406762102390689137313602282933279913445956640318460877596302561808395380461415106167065014357592743950744927372395465891910118252652212972302131134079050532451149754695077612377412527160230162604703887969647382897660268151635112753413107594654984035216995265525783277799783643397535391039259830075231307159816794447936215512540035941559909336271137104331877668558247630323284818216099815229953237756016600679798685436899509107715581885756148248843202915311300817133688783056711641444015014639170163307890882586270993194423467980404875463942226829451961698976781223086335166324815200485884133271147252573926116953198225213068989334314861309893801231329732005461451125016138607639611640276836775317465532819324946031927660432351841369875750042851579573472914075165984073436897888490000276817770201469415628190905214658539038118549124737801875694270593035696912396883568762853854418292004086688112916683553384220461200385305927369972639953528535104066061781619002714302604862756323638711212682097665061191072550346923951348604583564473087962981784294911892273480467825571995990170443798503987977728514039136042744044976002172318596897143283952167346519910297815516043860102687028733265921295161953142201759162359576566027998100461119182214029174147452823689978208067031271862768446307269539768752551951612572573190756580416321580183202378753570836134842441916255939691699918354751904048687928998567897441417354859338087808462826459708990432521954083187934071923467533050568585549550356174281476672912573764204905199855984701732459817003068198866050161540827995917183968111827951220496281276636930896788970584843441200148921806577247834236592837620058758931492893617580094428308010405262228025112117037976199176852606790663218100855898063428094191556308814936709052852736510514888719878382294758965254382725487974973250043170212065925080451306818437798733876481151453967947642537303845511920414908055432994346225660828195668409560592403592594220928181184680446725394311846824807972127511063434602284631544105350386093530053096789310235736169943077446922173605369903895502102007015734359310273475401964549362137698106711484044122236155205722186322901505035502622299234313275232234983092323939505333125736530555615345486380387099773982630464948233687184276459114153095751474085381553132899488640738911687853723662460920333640343823151234030144295566406961526582666396567138797637452462366261974335749549312216647216420591532135552572429554940828756601261393426762821170500329632082448725836527596543622323709698438385146940723446329811157055499751500352374429081654159534822381457097000681852912911826240679328123688369465856546977624760854199138774210155533947019434360239426266810298306218196358020359684083971320332049873260664624962704924421931299377649039477480572360799849420900858117966975158934970312332224244582149634679238177738337960922464032293766848410648124073921956431068555281538985525789756475152643911415907570028382175497713330805724792916080814710868396843933639597813032278009734188051659702241648631092626733806931232836520263599493101728424949161677276299075776039147888708958080863784871358489394658159924466667599554992876131760824950256552635456230496517080884656088076724152509147373914159152165686986094858668025993332536113885910729888531386394591248540362207023895133452725358528636443013199097160261073897890938138570494830737880376551906137798463916400065066840151302531872934553664230958928689213521837394122450184672457352914052196727989879514349986416558204245856674858280684643682552905509973136649238706372397038007294137423737761293433095619649978052485933254913092844873271874959399690463892754963078533218467270984722989903109112149382809221802527702814618337930565800650020237806719479426414198843257288025636878693589300142208716349177396035557101489834288634381674118173935679061020392290896633826042304264722495038408759404777603834834337722282988894101301372679267570057075983158684850522258432233698439689056602125489847462975954061255119889674470100511898132753513438894328651482518154839590629201509294510221297344949674386669059576384216636569484382303543194160284764820481441263448023229068842967336971326142769387983918485420681668598477985248318591164921714419191879802183228222572737422092922397467317449880002644224983091209369063914416033593753045400697341622681629252331326566725955230849302528634201604486342522475996954503457695839742435505924228097155008583750664926855632812271989723217178277360132792833638032465644610201189567783550802221853422519935869758683055648251409963336271250818957108985119611243497237016310752832646814619303231590315401546394793188119226542119373744932133481790099653460958441487671309504140522622388155912177691893018155638572107546151563744024541833336644253880693388371053514352887345239471270300912392215210078044555068653795920521496827842891393118576921090341851958852973653478207355396543102510195772632135688431002351627652798274170174646131181777904271776208095763125558455780726343467746174361958978603933027846472192956223596961759545117681564522710395216745953483968912232486721258138719885017669860206189395853191314740824982249000787750387882002315931283307552259758800147230441364940067737629724267789495133437902185231440687109846850636144380099873463638315148991728298122033356354517921412768790740865868179278375935463048392519469202622915045481412671241987421006155396670134490999379703029675174589548207343841650869981851325196241444047250234125599598103239998585995003435281999839335940176071824247962961389497089327327497725429986186668543467719894908942685390446320023030644494888792734335849898918538685688454053767908976296753379165477119577825897131267426873248687497237940676210239068913731360228293327991344595664031846087759630256180839538046141510616706501435759274395074492737239546589191011825265221297230213113407905053245114975469507761237741252716023016260470388796964738289766026815163511275341310759465498403521699526552578327779978364339753539103925983007523130715981679444793621551254003594155990933627113710433187766855824763032328481821609981522995323775601660067979868543689950910771558188575614824884320291531130081713368878305671164144401501463917016330789088258627099319442346798040487546394222682945196169897678122308633516632481520048588413327114725257392611695319822521306898933431486130989380123132973200546145112501613860763961164027683677531746553281932494603192766043235184136987575004285157957347291407516598407343689788849000027681777020146941562819090521465853903811854912473780187569427059303569691239688356876285385441829200408668811291668355338422046120038530592736997263995352853510406606178161900271430260486275632363871121268209766506119107255034692395134860458356447308796298178429491189227348046782557199599017044379850398797772851403913604274404497600217231859689714328395216734651991029781551604386010268702873326592129516195314220175916235957656602799810046111918221402917414745282368997820806703127186276844630726953976875255195161257257319075658041632158018320237875357083613484244191625593969169991835475190404868792899856789744141735485933808780846282645970899043252195408318793407192346753305056858554955035617428147667291257376420490519985598470173245981700306819886605016154082799591718396811182795122049628127663693089678897058484344120014892180657724783423659283762005875893149289361758009442830801040526222802511211703797619917685260679066321810085589806342809419155630881493670905285273651051488871987838229475896525438272548797497325004317021206592508045130681843779873387648115145396794764253730384551192041490805543299434622566082819566840956059240359259422092818118468044672539431184682480797212751106343460228463154410535038609353005309678931023573616994307744692217360536990389550210200701573435931027347540196454936213769810671148404412223615520572218632290150503550262229923431327523223498309232393950533312573653055561534548638038709977398263046494823368718427645911415309575147408538155313289948864073891168785372366246092033364034382315123403014429556640696152658266639656713879763745246236626197433574954931221664721642059153213555257242955494082875660126139342676282117050032963208244872583652759654362232370969843838514694072344632981115705549975150035237442908165415953482238145709700068185291291182624067932812368836946585654697762476085419913877421015553394701943436023942626681029830621819635802035968408397132033204987326066462496270492442193129937764903947748057236079984942090085811796697515893497031233222424458214963467923817773833796092246403229376684841064812407392195643106855528153898552578975647515264391141590757002838217549771333080572479291608081471086839684393363959781303227800973418805165970224164863109262673380693123283652026359949310172842494916167727629907577603914788870895808086378487135848939465815992446666759955499287613176082495025655263545623049651708088465608807672415250914737391415915216568698609485866802599333253611388591072988853138639459124854036220702389513345272535852863644301319909716026107389789093813857049483073788037655190613779846391640006506684015130253187293455366423095892868921352183739412245018467245735291405219672798987951434998641655820424585667485828068464368255290550997313664923870637239703800729413742373776129343309561964997805248593325491309284487327187495939969046389275496307853321846727098472298990310911214938280922180252770281461833793056580065002023780671947942641419884325728802563687869358930014220871634917739603555710148983428863438167411817393567906102039229089663382604230426472249503840875940477760383483433772228298889410130137267926757005707598315868485052225843223369843968905660212548984746297595406125511988967447010051189813275351343889432865148251815483959062920150929451022129734494967438666905957638421663656948438230354319416028476482048144126344802322906884296733697132614276938798391848542068166859847798524831859116492171441919187980218322822257273742209292239746731744988000264422498309120936906391441603359375304540069734162268162925233132656672595523084930252863420160448634252247599695450345769583974243550592422809715500858375066492685563281227198972321717827736013279283363803246564461020118956778355080222185342251993586975868305564825140996333627125081895710898511961124349723701631075283264681461930323159031540154639479318811922654211937374493213348179009965346095844148767130950414052262238815591217769189301815563857210754615156374402454183333664425388069338837105351435288734523947127030091239221521007804455506865379592052149682784289139311857692109034185195885297365347820735539654310251019577263213568843100235162765279827417017464613118177790427177620809576312555845578072634346774617436195897860393302784647219295622359696175954511768156452271039521674595348396891223248672125813871988501766986020618939585319131474082498224900078775038788200231593128330755225");
	bigint b("-97588001472304413649400677376297242677894951334379021852314406871098468506361443800998734636383151489917282981220333563545179214127687907408658681792783759354630483925194692026229150454814126712419874210061553966701344909993797030296751745895482073438416508699818513251962414440472502341255995981032399985859950034352819998393359401760718242479629613894970893273274977254299861866685434677198949089426853904463200230306444948887927343358498989185386856884540537679089762967533791654771195778258971312674268732486874972379406762102390689137313602282933279913445956640318460877596302561808395380461415106167065014357592743950744927372395465891910118252652212972302131134079050532451149754695077612377412527160230162604703887969647382897660268151635112753413107594654984035216995265525783277799783643397535391039259830075231307159816794447936215512540035941559909336271137104331877668558247630323284818216099815229953237756016600679798685436899509107715581885756148248843202915311300817133688783056711641444015014639170163307890882586270993194423467980404875463942226829451961698976781223086335166324815200485884133271147252573926116953198225213068989334314861309893801231329732005461451125016138607639611640276836775317465532819324946031927660432351841369875750042851579573472914075165984073436897888490000276817770201469415628190905214658539038118549124737801875694270593035696912396883568762853854418292004086688112916683553384220461200385305927369972639953528535104066061781619002714302604862756323638711212682097665061191072550346923951348604583564473087962981784294911892273480467825571995990170443798503987977728514039136042744044976002172318596897143283952167346519910297815516043860102687028733265921295161953142201759162359576566027998100461119182214029174147452823689978208067031271862768446307269539768752551951612572573190756580416321580183202378753570836134842441916255939691699918354751904048687928998567897441417354859338087808462826459708990432521954083187934071923467533050568585549550356174281476672912573764204905199855984701732459817003068198866050161540827995917183968111827951220496281276636930896788970584843441200148921806577247834236592837620058758931492893617580094428308010405262228025112117037976199176852606790663218100855898063428094191556308814936709052852736510514888719878382294758965254382725487974973250043170212065925080451306818437798733876481151453967947642537303845511920414908055432994346225660828195668409560592403592594220928181184680446725394311846824807972127511063434602284631544105350386093530053096789310235736169943077446922173605369903895502102007015734359310273475401964549362137698106711484044122236155205722186322901505035502622299234313275232234983092323939505333125736530555615345486380387099773982630464948233687184276459114153095751474085381553132899488640738911687853723662460920333640343823151234030144295566406961526582666396567138797637452462366261974335749549312216647216420591532135552572429554940828756601261393426762821170500329632082448725836527596543622323709698438385146940723446329811157055499751500352374429081654159534822381457097000681852912911826240679328123688369465856546977624760854199138774210155533947019434360239426266810298306218196358020359684083971320332049873260664624962704924421931299377649039477480572360799849420900858117966975158934970312332224244582149634679238177738337960922464032293766848410648124073921956431068555281538985525789756475152643911415907570028382175497713330805724792916080814710868396843933639597813032278009734188051659702241648631092626733806931232836520263599493101728424949161677276299075776039147888708958080863784871358489394658159924466667599554992876131760824950256552635456230496517080884656088076724152509147373914159152165686986094858668025993332536113885910729888531386394591248540362207023895133452725358528636443013199097160261073897890938138570494830737880376551906137798463916400065066840151302531872934553664230958928689213521837394122450184672457352914052196727989879514349986416558204245856674858280684643682552905509973136649238706372397038007294137423737761293433095619649978052485933254913092844873271874959399690463892754963078533218467270984722989903109112149382809221802527702814618337930565800650020237806719479426414198843257288025636878693589300142208716349177396035557101489834288634381674118173935679061020392290896633826042304264722495038408759404777603834834337722282988894101301372679267570057075983158684850522258432233698439689056602125489847462975954061255119889674470100511898132753513438894328651482518154839590629201509294510221297344949674386669059576384216636569484382303543194160284764820481441263448023229068842967336971326142769387983918485420681668598477985248318591164921714419191879802183228222572737422092922397467317449880002644224983091209369063914416033593753045400697341622681629252331326566725955230849302528634201604486342522475996954503457695839742435505924228097155008583750664926855632812271989723217178277360132792833638032465644610201189567783550802221853422519935869758683055648251409963336271250818957108985119611243497237016310752832646814619303231590315401546394793188119226542119373744932133481790099653460958441487671309504140522622388155912177691893018155638572107546151563744024541833336644253880693388371053514352887345239471270300912392215210078044555068653795920521496827842891393118576921090341851958852973653478207355396543102510195772632135688431002351627652798274170174646131181777904271776208095763125558455780726343467746174361958978603933027846472192956223596961759545117681564522710395216745953483968912232486721258138719885017669860206189395853191314740824982249000787750387882002315931283307552259758800147230441364940067737629724267789495133437902185231440687109846850636144380099873463638315148991728298122033356354517921412768790740865868179278375935463048392519469202622915045481412671241987421006155396670134490999379703029675174589548207343841650869981851325196241444047250234125599598103239998585995003435281999839335940176071824247962961389497089327327497725429986186668543467719894908942685390446320023030644494888792734335849898918538685688454053767908976296753379165477119577825897131267426873248687497237940676210239068913731360228293327991344595664031846087759630256180839538046141510616706501435759274395074492737239546589191011825265221297230213113407905053245114975469507761237741252716023016260470388796964738289766026815163511275341310759465498403521699526552578327779978364339753539103925983007523130715981679444793621551254003594155990933627113710433187766855824763032328481821609981522995323775601660067979868543689950910771558188575614824884320291531130081713368878305671164144401501463917016330789088258627099319442346798040487546394222682945196169897678122308633516632481520048588413327114725257392611695319822521306898933431486130989380123132973200546145112501613860763961164027683677531746553281932494603192766043235184136987575004285157957347291407516598407343689788849000027681777020146941562819090521465853903811854912473780187569427059303569691239688356876285385441829200408668811291668355338422046120038530592736997263995352853510406606178161900271430260486275632363871121268209766506119107255034692395134860458356447308796298178429491189227348046782557199599017044379850398797772851403913604274404497600217231859689714328395216734651991029781551604386010268702873326592129516195314220175916235957656602799810046111918221402917414745282368997820806703127186276844630726953976875255195161257257319075658041632158018320237875357083613484244191625593969169991835475190404868792899856789744141735485933808780846282645970899043252195408318793407192346753305056858554955035617428147667291257376420490519985598470173245981700306819886605016154082799591718396811182795122049628127663693089678897058484344120014892180657724783423659283762005875893149289361758009442830801040526222802511211703797619917685260679066321810085589806342809419155630881493670905285273651051488871987838229475896525438272548797497325004317021206592508045130681843779873387648115145396794764253730384551192041490805543299434622566082819566840956059240359259422092818118468044672539431184682480797212751106343460228463154410535038609353005309678931023573616994307744692217360536990389550210200701573435931027347540196454936213769810671148404412223615520572218632290150503550262229923431327523223498309232393950533312573653055561534548638038709977398263046494823368718427645911415309575147408538155313289948864073891168785372366246092033364034382315123403014429556640696152658266639656713879763745246236626197433574954931221664721642059153213555257242955494082875660126139342676282117050032963208244872583652759654362232370969843838514694072344632981115705549975150035237442908165415953482238145709700068185291291182624067932812368836946585654697762476085419913877421015553394701943436023942626681029830621819635802035968408397132033204987326066462496270492442193129937764903947748057236079984942090085811796697515893497031233222424458214963467923817773833796092246403229376684841064812407392195643106855528153898552578975647515264391141590757002838217549771333080572479291608081471086839684393363959781303227800973418805165970224164863109262673380693123283652026359949310172842494916167727629907577603914788870895808086378487135848939465815992446666759955499287613176082495025655263545623049651708088465608807672415250914737391415915216568698609485866802599333253611388591072988853138639459124854036220702389513345272535852863644301319909716026107389789093813857049483073788037655190613779846391640006506684015130253187293455366423095892868921352183739412245018467245735291405219672798987951434998641655820424585667485828068464368255290550997313664923870637239703800729413742373776129343309561964997805248593325491309284487327187495939969046389275496307853321846727098472298990310911214938280922180252770281461833793056580065002023780671947942641419884325728802563687869358930014220871634917739603555710148983428863438167411817393567906102039229089663382604230426472249503840875940477760383483433772228298889410130137267926757005707598315868485052225843223369843968905660212548984746297595406125511988967447010051189813275351343889432865148251815483959062920150929451022129734494967438666905957638421663656948438230354319416028476482048144126344802322906884296733697132614276938798391848542068166859847798524831859116492171441919187980218322822257273742209292239746731744988000264422498309120936906391441603359375304540069734162268162925233132656672595523084930252863420160448634252247599695450345769583974243550592422809715500858375066492685563281227198972321717827736013279283363803246564461020118956778355080222185342251993586975868305564825140996333627125081895710898511961124349723701631075283264681461930323159031540154639479318811922654211937374493213348179009965346095844148767130950414052262238815591217769189301815563857210754615156374402454183333664425388069338837105351435288734523947127030091239221521007804455506865379592052149682784289139311857692109034185195885297365347820735539654310251019577263213568843100235162765279827417017464613118177790427177620809576312555845578072634346774617436195897860393302784647219295622359696175954511768156452271039521674595348396891223248672125813871988501766986020618939585319131474082498224900078775038788200231593128330755225");
	cout << a << endl;
	a += b;
	cout << a << endl;
}

int main() {
	ofstream file("test.txt");
	bigint x, y("-1234"), z("+00000987654321");

	bigint a = -1234;
	bigint b = 456;
	file << "hello" << a << " " << b << endl;
	file.close();

	a += b; cout << a << endl;

	cout << x << " " << y << " " << z << endl;

	x = y;
	z = y;
	cout << x << ' ' << y << ' ' << z << endl;

	x = -123, y = -123; z = 123;
	cout << x << ' ' << y << ' ' << z << endl;

	x = 123; y = -123; z = -99;
	x += z; y += z;
	cout << x << ' ' << y << ' ' << z << endl;

	x = 123; y = -123; z = 99;
	x += z; y += z;
	cout << x << ' ' << y << ' ' << z << endl;

	x = 99;
	++x; y = x;
	x++; z = x;
	cout << x << ' ' << y << ' ' << z << endl;  //101 100 100

	x = -99;
	++x; y = x;
	x++; z = x;
	cout << x << " " << y << " " << z << endl;

	x = -100;
	++x; y = x;
	x++; z = x;
	cout << x << ' ' << y << ' ' << z << endl;	//-98 -99 -98
	test();
}