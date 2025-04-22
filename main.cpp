#include <print>
#include "quine_mccluskey.hpp"

int main() {
	using namespace zikken2a3::stew;

	const auto bit_map_str = std::vector<std::string> {
		"3", "2", "1", "0"
	};
	
	const std::vector<u8> a_trues {
		0,
		2,
		3,
		5,
		6,
		7,
		8,
		9
	};

	const std::vector<u8> a_dcs {
		0,
		2,
		3,
		5,
		6,
		7,
		8,
		9,
		10,
		11,
		12,
		13,
		14,
		15
	};

	const std::vector<u8> b_trues {
		0,
		1,
		2,
		3,
		4,
		7,
		8,
		9
	};

	const std::vector<u8> b_dcs {
		0,
		1,
		2,
		3,
		4,
		7,
		8,
		9,
		10,
		11,
		12,
		13,
		14,
		15
	};

	const std::vector<u8> c_trues {
		0,
		1,
		3,
		4,
		5,
		6,
		7,
		8,
		9
	};

	const std::vector<u8> c_dcs {
		0,
		1,
		3,
		4,
		5,
		6,
		7,
		8,
		9,

		10,
		11,
		12,
		13,
		14,
		15
	};

	const std::vector<u8> d_trues {
		0,
		2,
		3,
		5,
		6,
		8,
		9
	};

	const std::vector<u8> d_dcs {
		0,
		2,
		3,
		5,
		6,
		8,
		9,
		10,
		11,
		12,
		13,
		14,
		15
	};

	const std::vector<u8> e_trues {
		0,
		2,
		6,
		8
	};

	const std::vector<u8> e_dcs {
		0,
		2,
		6,
		8,
		10,
		11,
		12,
		13,
		14,
		15
	};

	const std::vector<u8> f_trues {
		0,
		4,
		5,
		6,
		7,
		8,
		9
	};

	const std::vector<u8> f_dcs {
		0,
		4,
		5,
		6,
		7,
		8,
		9,
		10,
		11,
		12,
		13,
		14,
		15
	};

	const std::vector<u8> g_trues {
		2,
		3,
		4,
		5,
		6,
		8,
		9
	};

	const std::vector<u8> g_dcs {
		2,
		3,
		4,
		5,
		6,
		8,
		9,
		10,
		11,
		12,
		13,
		14,
		15
	};

	const auto a_ = get_main_terms(4, a_dcs, a_trues);
	const auto b_ = get_main_terms(4, b_dcs, b_trues);
	const auto c_ = get_main_terms(4, c_dcs, c_trues);
	const auto d_ = get_main_terms(4, d_dcs, d_trues);
	const auto e_ = get_main_terms(4, e_dcs, e_trues);
	const auto f_ = get_main_terms(4, f_dcs, f_trues);
	const auto g_ = get_main_terms(4, g_dcs, g_trues);
	
	auto pr = [&bit_map_str](const std::vector<Term>& terms) -> void {
		for(const auto& term : terms) {
			std::print("{} + ", term.as_str(bit_map_str));
		}
		std::println();
	};
	
	pr(a_);
	pr(b_);
	pr(c_);
	pr(d_);
	pr(e_);
	pr(f_);
	pr(g_);
	std::print("done\n");
}