#include "SPLayer.h"
using std::hex;
using std::dec;

void SPLayer::gen_table_lookup() {
	int input_size = 1 << get_ibn();
	for (int sbox_input = 0; sbox_input < input_size; sbox_input++) {
		for (int sbox_index = 0; sbox_index < get_sn(); sbox_index++) {
			for (int weight_index = 0; weight_index < get_weight_vec_size(); weight_index++) {
				for (int output_offset = 0; output_offset < get_output_size(sbox_input, weight_index); output_offset++) {
					_sp_table_lookup[sbox_input][sbox_index][weight_index].emplace_back(get_p_at(get_output_at(sbox_input, weight_index, output_offset), sbox_index));
					State s(get_sn());
					s.set(sbox_index, get_output_at(sbox_input, weight_index, output_offset));
					_s_table_lookup[sbox_input][sbox_index][weight_index].emplace_back(s);
				}
			}
		}
	}
}

ostream& operator<<(ostream& out, const SPLayer& obj) {
	out << "SP±í£º" << "\n";
	for (int x = 0; x < (1<<obj.get_ibn()); x++) {
		for (int i = 0; i < obj.get_sn(); i++) {
			out << hex << "input: " << x << dec << "\tindex: " << i << "\n";
			State si(obj.get_sn());
			si.set(i, x);
			for (int w = 0; w < obj.get_weight_vec_size(); w++) {
				out << dec << "#w(" << w << ")=" << obj.get_sp_size(x,i,w) << ": ";
				out << hex;
				for (int z = 0; z < obj.get_output_size(x, w); z++) out << (int)obj.get_output_at(x, w, z) << " ";
				for (int y = 0; y < obj.get_sp_size(x, i, w); y++) {
					out << "\n\t";
					out << "<" << si << ">";
					out << " S ";
					out << "<" << obj.get_s_at(x, i, w, y) << ">";
					out << " P ";
					out << "<" << obj.get_sp_at(x, i, w, y) << ">";
				}
				out << "\n";
			}
			out << "\n";
		}
	}
	return out;
}
