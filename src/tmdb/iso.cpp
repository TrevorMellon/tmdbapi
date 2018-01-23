#include <tmdb/iso.h>

#include <map>
#include <stdint.h>
#include <utility>

using namespace iso;
namespace iso
{

	Languages isotable[] =
	{
		"ab", iso_639_1::iso_ab, "abk", iso_639_2::iso_abk, "Abkhazian", "Northwest Caucasian",
		"aa", iso_639_1::iso_aa, "aar", iso_639_2::iso_aar, "Afar", "Afro - Asiatic",
		"af", iso_639_1::iso_af, "afr", iso_639_2::iso_afr, "Afrikaans", "Indo - European",
		"ak", iso_639_1::iso_ak, "aka", iso_639_2::iso_aka, "Akan", "Niger–Congo",
		"sq", iso_639_1::iso_sq, "sqi", iso_639_2::iso_sqi, "Albanian", "Indo - European",
		"am", iso_639_1::iso_am, "amh", iso_639_2::iso_amh, "Amharic", "Afro - Asiatic",
		"ar", iso_639_1::iso_ar, "ara", iso_639_2::iso_ara, "Arabic", "Afro - Asiatic",
		"an", iso_639_1::iso_an, "arg", iso_639_2::iso_arg, "Aragonese", "Indo - European",
		"hy", iso_639_1::iso_hy, "hye", iso_639_2::iso_hye, "Armenian", "Indo - European",
		"as", iso_639_1::iso_as, "asm", iso_639_2::iso_asm, "Assamese", "Indo - European",
		"av", iso_639_1::iso_av, "ava", iso_639_2::iso_ava, "Avaric", "Northeast Caucasian",
		"ae", iso_639_1::iso_ae, "ave", iso_639_2::iso_ave, "Avestan", "Indo - European",
		"ay", iso_639_1::iso_ay, "aym", iso_639_2::iso_aym, "Aymara", "Aymaran",
		"az", iso_639_1::iso_az, "aze", iso_639_2::iso_aze, "Azerbaijani", "Turkic",
		"bm", iso_639_1::iso_bm, "bam", iso_639_2::iso_bam, "Bambara", "Niger-Congo",
		"ba", iso_639_1::iso_ba, "bak", iso_639_2::iso_bak, "Baskir", "Turkic",
		"eu", iso_639_1::iso_eu, "eus", iso_639_2::iso_eus, "Basque", "Language Isolate",
		"be", iso_639_1::iso_be, "bel", iso_639_2::iso_bel, "Belarussian", "Indo - European",
		"bn", iso_639_1::iso_bn, "ben", iso_639_2::iso_ben, "Bengali", "Indo - European",
		"bh", iso_639_1::iso_bh, "bih", iso_639_2::iso_bih, "Bihari", "Indo - European",
		"bi", iso_639_1::iso_bi, "bis", iso_639_2::iso_bis, "Bislama", "Creole",
		"bs", iso_639_1::iso_bs, "bos", iso_639_2::iso_bos, "Bosnian", "Indo - European",
		"br", iso_639_1::iso_br, "bre", iso_639_2::iso_bre, "Breton", "Indo - European",
		"bg", iso_639_1::iso_bg, "bul", iso_639_2::iso_bul, "Bulgarian", "Indo - European",
		"my", iso_639_1::iso_my, "mya", iso_639_2::iso_mya, "Burmese", "Sino - Tibetan",
		"ca", iso_639_1::iso_ca, "cat", iso_639_2::iso_cat, "Catalan", "Indo - European",
		"ch", iso_639_1::iso_ch, "cha", iso_639_2::iso_cha, "Chamorro", "Austronesian",
		"ce", iso_639_1::iso_ce, "che", iso_639_2::iso_che, "Chechen", "Northest Caucasian",
		"ny", iso_639_1::iso_ny, "nya", iso_639_2::iso_nya, "Chichewa", "Niger-Congo",
		"zh", iso_639_1::iso_zh, "zho", iso_639_2::iso_zho, "Chinese", "Sino - Tibetan",
		"cv", iso_639_1::iso_cv, "chv", iso_639_2::iso_chv, "Chuvash", "Turkic",
		"kw", iso_639_1::iso_kw, "cor", iso_639_2::iso_cor, "Cornish", "Indo - European",
		"co", iso_639_1::iso_co, "cos", iso_639_2::iso_cos, "Corsican", "Indo - European",
		"cr", iso_639_1::iso_cr, "cre", iso_639_2::iso_cre, "Cree", "Algonquian",
		"hr", iso_639_1::iso_hr, "hrv", iso_639_2::iso_hrv, "Croatian", "Indo - European",
		"cs", iso_639_1::iso_cs, "ces", iso_639_2::iso_ces, "Czech", "Indo - European",
		"da", iso_639_1::iso_da, "dan", iso_639_2::iso_dan, "Danish", "Indo - European",
		"dv", iso_639_1::iso_dv, "div", iso_639_2::iso_div, "Divehi", "Indo - European",
		"nl", iso_639_1::iso_nl, "nld", iso_639_2::iso_nld, "Dutch", "Indo - European",
		"dz", iso_639_1::iso_dz, "dzo", iso_639_2::iso_dzo, "Tibetan", "Sino - Tibetan",
		"en", iso_639_1::iso_en, "eng", iso_639_2::iso_eng, "English", "Indo - European",
		"eo", iso_639_1::iso_eo, "epo", iso_639_2::iso_epo, "Esperanto", "Constructed",
		"et", iso_639_1::iso_et, "est", iso_639_2::iso_est, "Estonian", "Uralic",
		"ee", iso_639_1::iso_ee, "ewe", iso_639_2::iso_ewe, "Ewe", "Niger-Congo",
		"fo", iso_639_1::iso_fo, "fao", iso_639_2::iso_fao, "Faroese", "Indo - European",
		"fj", iso_639_1::iso_fj, "fij", iso_639_2::iso_fij, "Fijian", "Austronesian",
		"fi", iso_639_1::iso_fi, "fin", iso_639_2::iso_fin, "Finnish", "Uralic",
		"fr", iso_639_1::iso_fr, "fra", iso_639_2::iso_fra, "French", "Indo - European",
		"ff", iso_639_1::iso_ff, "ful", iso_639_2::iso_ful, "Fulah", "Niger-Congo",
		"gl", iso_639_1::iso_gl, "glg", iso_639_2::iso_glg, "Galician", "Indo - European",
		"ka", iso_639_1::iso_ka, "kat", iso_639_2::iso_kat, "Georgian", "Indo - European",
		"de", iso_639_1::iso_de, "deu", iso_639_2::iso_deu, "German", "Indo - European",
		"el", iso_639_1::iso_el, "ell", iso_639_2::iso_ell, "Greek (Modern)", "Indo - European",
		"gn", iso_639_1::iso_gn, "grn", iso_639_2::iso_grn, "Gurani", "Tupian",
		"ht", iso_639_1::iso_ht, "hat", iso_639_2::iso_hat, "Haitian", "Creole",
		"ha", iso_639_1::iso_ha, "hau", iso_639_2::iso_hau, "Hausa", "Afro - Asiatic",
		"he", iso_639_1::iso_he, "heb", iso_639_2::iso_heb, "Hebew (Modern)", "Afro - Asiatic",
		"hz", iso_639_1::iso_hz, "her", iso_639_2::iso_her, "Herero", "Niger-Congo",
		"hi", iso_639_1::iso_hi, "hin", iso_639_2::iso_hin, "Hindi", "Indo - European",
		"ho", iso_639_1::iso_ho, "hmo", iso_639_2::iso_hmo, "Hiri Moto", "Austronesian",
		"hu", iso_639_1::iso_hu, "hun", iso_639_2::iso_hun, "Hungarian", "Uralic",
		"ia", iso_639_1::iso_ia, "ina", iso_639_2::iso_ina, "Interlingua", "Constructed",
		"id", iso_639_1::iso_id, "ind", iso_639_2::iso_ind, "Indonesian Bahasa", "Austonesian",
		"ie", iso_639_1::iso_ie, "ile", iso_639_2::iso_ile, "Interlingue", "Constructed",
		"ga", iso_639_1::iso_ga, "gle", iso_639_2::iso_gle, "Gaelige", "Indo - European",
		"ig", iso_639_1::iso_ig, "ibo", iso_639_2::iso_ibo, "Igbo", "Niger-Congo",
		"ik", iso_639_1::iso_ik, "ipk", iso_639_2::iso_ipk, "Inupiaq", "Eskimo-Aleut",
		"io", iso_639_1::iso_io, "ido", iso_639_2::iso_ido, "Ido", "Constructed",
		"is", iso_639_1::iso_is, "isl", iso_639_2::iso_isl, "Icelandic", "Indo - European",
		"it", iso_639_1::iso_it, "ita", iso_639_2::iso_ita, "Italian", "Indo - European",
		"iu", iso_639_1::iso_iu, "iku", iso_639_2::iso_iku, "Inukitut", "Eskimo-Aleut",
		"ja", iso_639_1::iso_ja, "jpn", iso_639_2::iso_jpn, "Japonese", "Japonic",
		"jv", iso_639_1::iso_jv, "jav", iso_639_2::iso_jav, "Javanese", "Austronesian",
		"kl", iso_639_1::iso_kl, "kal", iso_639_2::iso_kal, "Kalaallisut", "Eskimo-Aleut",
		"kn", iso_639_1::iso_kn, "kan", iso_639_2::iso_kan, "Kannada", "Dravidian",
		"kr", iso_639_1::iso_kr, "kau", iso_639_2::iso_kau, "Kanuri", "Nilo - Saharan",
		"ks", iso_639_1::iso_ks, "kas", iso_639_2::iso_kas, "Kashmiri", "Indo - European",
		"kk", iso_639_1::iso_kk, "kaz", iso_639_2::iso_kaz, "Kazakh", "Turkic",
		"km", iso_639_1::iso_km, "khm", iso_639_2::iso_khm, "Khmer", "Austroasiatic",
		"ki", iso_639_1::iso_ki, "kik", iso_639_2::iso_kik, "Kikuyu", "Niger-Congo",
		"rw", iso_639_1::iso_rw, "kin", iso_639_2::iso_kin, "Kinyarwanda", "Niger-Congo",
		"ky", iso_639_1::iso_ky, "kir", iso_639_2::iso_kir, "Kyrgyz", "Turkic",
		"kv", iso_639_1::iso_kv, "kom", iso_639_2::iso_kom, "Komi", "Uralic",
		"kg", iso_639_1::iso_kg, "kon", iso_639_2::iso_kon, "Kongo", "Niger-Congo",
		"ko", iso_639_1::iso_ko, "kor", iso_639_2::iso_kor, "Korean", "Koreanic",
		"ku", iso_639_1::iso_ku, "kur", iso_639_2::iso_kur, "Kurdish", "Indo - European",
		"kj", iso_639_1::iso_kj, "kua", iso_639_2::iso_kua, "Kwanyama", "Niger-Congo",
		"la", iso_639_1::iso_la, "lat", iso_639_2::iso_lat, "Latin", "Indo - European",
		"lb", iso_639_1::iso_lb, "ltz", iso_639_2::iso_ltz, "Luxembourgish", "Indo - European",
		"lg", iso_639_1::iso_lg, "lug", iso_639_2::iso_lug, "Ganda", "Niger-Congo",
		"li", iso_639_1::iso_li, "lim", iso_639_2::iso_lim, "Limburgan", "Indo - European",
		"ln", iso_639_1::iso_ln, "lin", iso_639_2::iso_lin, "Lingala", "Niger-Congo",
		"lo", iso_639_1::iso_lo, "lao", iso_639_2::iso_lao, "Lao", "Tai-Kadai",
		"lt", iso_639_1::iso_lt, "lit", iso_639_2::iso_lit, "Lithuanian", "Indo - European",
		"lu", iso_639_1::iso_lu, "lub", iso_639_2::iso_lub, "Luba - Katanga", "Niger-Congo",
		"lv", iso_639_1::iso_lv, "lav", iso_639_2::iso_lav, "Latvian", "Indo - European",
		"gv", iso_639_1::iso_gv, "glv", iso_639_2::iso_glv, "Manx", "Indo - European",
		"mk", iso_639_1::iso_mk, "mkd", iso_639_2::iso_mkd, "MAcedonian", "Indo - European",
		"mg", iso_639_1::iso_mg, "mlg", iso_639_2::iso_mlg, "Malagasy", "Austonesian",
		"ms", iso_639_1::iso_ms, "msa", iso_639_2::iso_msa, "Malay", "Austonesian",
		"ml", iso_639_1::iso_ml, "mal", iso_639_2::iso_mal, "Malayalam", "Dravidian",
		"mt", iso_639_1::iso_mt, "mlt", iso_639_2::iso_mlt, "Maltese", "Afro - Asiatic",
		"mi", iso_639_1::iso_mi, "mri", iso_639_2::iso_mri, "Maori", "Austronesian",
		"mr", iso_639_1::iso_mr, "mar", iso_639_2::iso_mar, "Marathi", "Indo - European",
		"mh", iso_639_1::iso_mh, "mah", iso_639_2::iso_mah, "Marshallese", "Austronesian",
		"mn", iso_639_1::iso_mn, "mon", iso_639_2::iso_mon, "Mongolian", "Mongolic",
		"na", iso_639_1::iso_na, "nau", iso_639_2::iso_nau, "Nauru", "Austronesian",
		"nv", iso_639_1::iso_nv, "nav", iso_639_2::iso_nav, "Navajo", "Dene-Yenisian",
		"nd", iso_639_1::iso_nd, "nde", iso_639_2::iso_nde, "Ndebele", "Niger-Congo",
		"ne", iso_639_1::iso_ne, "nep", iso_639_2::iso_nep, "Nepali", "Indo - European",
		"ng", iso_639_1::iso_ng, "ndo", iso_639_2::iso_ndo, "Ndonga", "Niger-Congo",
		"nb", iso_639_1::iso_nb, "nob", iso_639_2::iso_nob, "Norwegian Bokmal", "Indo - European",
		"nn", iso_639_1::iso_nn, "nno", iso_639_2::iso_nno, "Norwegian Nynorsk", "Indo - European",
		"no", iso_639_1::iso_no, "nor", iso_639_2::iso_nor, "Norwegian", "Indo - European",
		"ii", iso_639_1::iso_ii, "iii", iso_639_2::iso_iii, "Sichuan Yi", "Sino - Tibetan",
		"nr", iso_639_1::iso_nr, "nbl", iso_639_2::iso_nbl, "South Ndebele", "Niger-Congo",
		"oc", iso_639_1::iso_oc, "oci", iso_639_2::iso_oci, "Occitan", "Indo - European",
		"oj", iso_639_1::iso_oj, "oji", iso_639_2::iso_oji, "Ojibwa", "Algonquian",
		"cu", iso_639_1::iso_cu, "chu", iso_639_2::iso_chu, "Church Slavonic", "Indo - European",
		"om", iso_639_1::iso_om, "orm", iso_639_2::iso_orm, "Oromo", "Afro - Asiatic",
		"or", iso_639_1::iso_or, "ori", iso_639_2::iso_ori, "Oriya", "Indo - European",
		"os", iso_639_1::iso_os, "oss", iso_639_2::iso_oss, "Ossetian", "Indo - European",
		"pa", iso_639_1::iso_pa, "pan", iso_639_2::iso_pan, "Punjabi", "Indo - European",
		"pi", iso_639_1::iso_pi, "pli", iso_639_2::iso_pli, "Pali", "Indo - European",
		"fa", iso_639_1::iso_fa, "fas", iso_639_2::iso_fas, "Persian", "Indo - European",
		"pl", iso_639_1::iso_pl, "pol", iso_639_2::iso_pol, "Polish", "Indo - European",
		"ps", iso_639_1::iso_ps, "pus", iso_639_2::iso_pus, "Pashto", "Indo - European",
		"pt", iso_639_1::iso_pt, "por", iso_639_2::iso_por, "Portuguese", "Indo - European",
		"qu", iso_639_1::iso_qu, "que", iso_639_2::iso_que, "Quechua Runa Simi", "Quechuan",
		"rm", iso_639_1::iso_rm, "roh", iso_639_2::iso_roh, "Romansh", "Indo - European",
		"rn", iso_639_1::iso_rn, "run", iso_639_2::iso_run, "Rundi", "Niger-Congo",
		"ro", iso_639_1::iso_ro, "ron", iso_639_2::iso_ron, "Romanian", "Indo - European",
		"ru", iso_639_1::iso_ru, "rus", iso_639_2::iso_rus, "Russian", "Indo - European",
		"sa", iso_639_1::iso_sa, "san", iso_639_2::iso_san, "Sanskrit", "Indo - European",
		"sc", iso_639_1::iso_sc, "srd", iso_639_2::iso_srd, "Sardinian", "Indo - European",
		"sd", iso_639_1::iso_sd, "snd", iso_639_2::iso_snd, "Sindhi", "Indo - European",
		"se", iso_639_1::iso_se, "sme", iso_639_2::iso_sme, "Northern Sami", "Uralic",
		"sm", iso_639_1::iso_sm, "smo", iso_639_2::iso_smo, "Samoan", "Austronesian",
		"sg", iso_639_1::iso_sg, "sag", iso_639_2::iso_sag, "Sango", "Creole",
		"sr", iso_639_1::iso_sr, "srp", iso_639_2::iso_srp, "Serbian", "Indo - European",
		"gd", iso_639_1::iso_gd, "gla", iso_639_2::iso_gla, "Gaelic Scottish", "Indo - European",
		"sn", iso_639_1::iso_sn, "sna", iso_639_2::iso_sna, "Shona", "Niger-Congo",
		"si", iso_639_1::iso_si, "sin", iso_639_2::iso_sin, "Sinhala", "Indo - European",
		"sk", iso_639_1::iso_sk, "slk", iso_639_2::iso_slk, "Slovak", "Indo - European",
		"sl", iso_639_1::iso_sl, "slv", iso_639_2::iso_slv, "Slovenian", "Indo - European",
		"so", iso_639_1::iso_so, "som", iso_639_2::iso_som, "Somali", "Afro - Asiatic",
		"st", iso_639_1::iso_st, "sot", iso_639_2::iso_sot, "Southern Sotho", "Niger-Congo",
		"es", iso_639_1::iso_es, "spa", iso_639_2::iso_spa, "Spanish", "Indo - European",
		"su", iso_639_1::iso_su, "sun", iso_639_2::iso_sun, "Sudanese", "Austronesian",
		"sw", iso_639_1::iso_sw, "swa", iso_639_2::iso_swa, "Swahili", "Niger-Congo",
		"ss", iso_639_1::iso_ss, "ssw", iso_639_2::iso_ssw, "Swati", "Niger-Congo",
		"sv", iso_639_1::iso_sv, "sse", iso_639_2::iso_sse, "Swedish", "Indo - European",
		"ta", iso_639_1::iso_ta, "tam", iso_639_2::iso_tam, "Tamil", "Dravidian",
		"te", iso_639_1::iso_te, "tel", iso_639_2::iso_tel, "Telugu", "Dravidian",
		"tg", iso_639_1::iso_tg, "tgk", iso_639_2::iso_tgk, "Tajik", "Indo - European",
		"th", iso_639_1::iso_th, "tha", iso_639_2::iso_tha, "Thai", "Tai-Kadai",
		"ti", iso_639_1::iso_ti, "tir", iso_639_2::iso_tir, "Tigrinya", "Afro - Asiatic",
		"bo", iso_639_1::iso_bo, "bod", iso_639_2::iso_bod, "Tibetan", "Sino - Tibetan",
		"tk", iso_639_1::iso_tk, "tuk", iso_639_2::iso_tuk, "Turkmen", "Turkic",
		"tl", iso_639_1::iso_tl, "tgl", iso_639_2::iso_tgl, "Tagalog", "Austronesian",
		"tn", iso_639_1::iso_tn, "tsn", iso_639_2::iso_tsn, "Tswana", "Niger-Congo",
		"to", iso_639_1::iso_to, "ton", iso_639_2::iso_ton, "Tonga", "Austonesian",
		"tr", iso_639_1::iso_tr, "tur", iso_639_2::iso_tur, "Turkish", "Turkic",
		"ts", iso_639_1::iso_ts, "tso", iso_639_2::iso_tso, "Tsonga", "Niger-Congo",
		"tt", iso_639_1::iso_tt, "tat", iso_639_2::iso_tat, "Tatar", "Turkic",
		"tw", iso_639_1::iso_tw, "twi", iso_639_2::iso_twi, "Twi", "Niger-Congo",
		"ty", iso_639_1::iso_ty, "tah", iso_639_2::iso_tah, "Tahitian", "Austonesian",
		"ug", iso_639_1::iso_ug, "uig", iso_639_2::iso_uig, "Uighur", "Turkic",
		"uk", iso_639_1::iso_uk, "ukr", iso_639_2::iso_ukr, "Ukranian", "Indo - European",
		"ur", iso_639_1::iso_ur, "urd", iso_639_2::iso_urd, "Urdu", "Indo - European",
		"uz", iso_639_1::iso_uz, "uzb", iso_639_2::iso_uzb, "Uzbek", "Turkic",
		"ve", iso_639_1::iso_ve, "ven", iso_639_2::iso_ven, "Venda", "Niger-Congo",
		"vi", iso_639_1::iso_vi, "vie", iso_639_2::iso_vie, "Vietnamese", "Austroasiatic",
		"vo", iso_639_1::iso_vo, "vol", iso_639_2::iso_vol, "Volapuk", "Constructed",
		"wa", iso_639_1::iso_wa, "wln", iso_639_2::iso_wln, "Walloon", "Indo - European",
		"cy", iso_639_1::iso_cy, "cym", iso_639_2::iso_cym, "Welsh", "Indo - European",
		"wo", iso_639_1::iso_wo, "wol", iso_639_2::iso_wol, "Wolof", "Niger-Congo",
		"fy", iso_639_1::iso_fy, "fry", iso_639_2::iso_fry, "Western Frisian", "Indo - European",
		"xh", iso_639_1::iso_xh, "xho", iso_639_2::iso_xho, "Xhosa", "Niger-Congo",
		"yi", iso_639_1::iso_yi, "yid", iso_639_2::iso_yid, "Yiddish", "Indo - European",
		"yo", iso_639_1::iso_yo, "yor", iso_639_2::iso_yor, "Yoruba", "Niger-Congo",
		"za", iso_639_1::iso_za, "zha", iso_639_2::iso_zha, "Zhuang", "Tai-Kadai",
		"zu", iso_639_1::iso_zu, "zul", iso_639_2::iso_zul, "Zulu", "Niger-Congo",
		"", iso_639_1::iso_unset, "", iso_639_2::iso_unset, "", "",
	};
}
namespace iso
{ 
	class IsoManagerPrivate
	{
	public:
		IsoManagerPrivate()
		{
			buildIndex();
		}

		void buildIndex()
		{
			Languages *l = nullptr;

			l = &isotable[0];

			uint32_t idx = 0;

			while (!l->iso_639_1_s.empty() &&
				!l->iso_639_2_s.empty() &&
				l->iso_639_1 != iso_639_1::iso_unset &&
				l->iso_639_2 != iso_639_2::iso_unset)
			{
				_iso_639_1_s_map.insert(std::make_pair(l->iso_639_1_s, l));
				_iso_639_2_s_map.insert(std::make_pair(l->iso_639_2_s, l));

				_iso_639_1_map.insert(std::make_pair(l->iso_639_1, l));
				_iso_639_2_map.insert(std::make_pair(l->iso_639_2, l));
			}
		}

		iso_639_1::iso getIso(const iso_639_2::iso &i)
		{
			auto iter = _iso_639_2_map.find(i);

			if (iter != _iso_639_2_map.end())
			{
				Languages *l = iter->second;
				return l->iso_639_1;
			}
			return iso_639_1::iso_error;
		}

		iso_639_2::iso getIso(const iso_639_1::iso &i)
		{
			auto iter = _iso_639_1_map.find(i);

			if (iter != _iso_639_1_map.end())
			{
				Languages *l = iter->second;
				return l->iso_639_2;
			}
			return iso_639_2::iso_error;
		}

		std::string getString(const iso_639_1::iso &i)
		{
			auto iter = _iso_639_1_map.find(i);

			if (iter != _iso_639_1_map.end())
			{
				Languages *l = iter->second;
				return l->iso_639_1_s;
			}
			return "";
		}

		std::string getString(const iso_639_2::iso &i)
		{
			auto iter = _iso_639_2_map.find(i);

			if (iter != _iso_639_2_map.end())
			{
				Languages *l = iter->second;
				return l->iso_639_2_s;
			}
			return "";
		}

		iso_639_1::iso to_639_1(const std::string &i)
		{
			auto iter = _iso_639_1_s_map.find(i);

			if (iter != _iso_639_1_s_map.end())
			{
				Languages *l = iter->second;
				return l->iso_639_1;
			}
			return iso_639_1::iso_error;
		}

		iso_639_2::iso to_639_2(const std::string &i)
		{
			auto iter = _iso_639_2_s_map.find(i);

			if (iter != _iso_639_2_s_map.end())
			{
				Languages *l = iter->second;
				return l->iso_639_2;
			}
			return iso_639_2::iso_error;
		}

	public:
		std::map<std::string, Languages*> _iso_639_1_s_map;
		std::map<std::string, Languages*> _iso_639_2_s_map;
		std::map<iso_639_1::iso, Languages*> _iso_639_1_map;
		std::map<iso_639_2::iso, Languages*> _iso_639_2_map;
	};
}

IsoManager::IsoManager()
{
	_p = new IsoManagerPrivate();

}
IsoManager::~IsoManager()
{
	if (_p)
	{
		delete _p;
	}
	_p = nullptr;
}

iso_639_1::iso IsoManager::getIso(const iso_639_2::iso &i)
{
	return _p->getIso(i);
}

iso_639_2::iso IsoManager::getIso(const iso_639_1::iso &i)
{
	return _p->getIso(i);
}

std::string IsoManager::getString(const iso_639_1::iso &i)
{
	return _p->getString(i);
}

std::string IsoManager::getString(const iso_639_2::iso &i)
{
	return _p->getString(i);
}

iso_639_1::iso IsoManager::to_639_1(const std::string &i)
{
	if (i.size() != 2)
	{
		return iso_639_1::iso_error;
	}
	return _p->to_639_1(i);
}

iso_639_2::iso IsoManager::to_639_2(const std::string &i)
{
	if (i.size() != 3)
	{
		return iso_639_2::iso_error;
	}
	return _p->to_639_2(i);
}
