export module chat;
export import <string>;

namespace skribbl {
	export class Chat {
	public:
		void isMatch(); //player-ul a gasit cuvantul
		void addListener();
		void removeListener();
		void Notify();  //player-ul a fost afk un anumit timp

	private:
		uint16_t characterLimit;
		uint8_t spamLimit;
	};
}