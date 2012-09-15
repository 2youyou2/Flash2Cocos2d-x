package effects{
	import flash.display.BlendMode;
	import flash.display.Sprite;
	import flash.events.Event;

	import flash.filters.GlowFilter;

	/**
	 * ...
	 * @author akdcl
	 */
	public class ShadowContainer extends Sprite {
		private var sd:Shadow;
		private static const GLOW_FILTER:GlowFilter = new GlowFilter(0x00ff00, 1, 8, 8, 1);

		public function ShadowContainer() {
			sd = new Shadow();
			sd.blendMode = BlendMode.LIGHTEN;
			sd.filters = [GLOW_FILTER];

			sd.startX = 0;
			sd.endX = -70;
			sd.startY = 0;
			sd.endY = 0;
			
			sd.shadowLength = 10;
			sd.shadowType = 1;
			sd.color = 0x00ff00;
			addChild(sd);
		}
	}

}