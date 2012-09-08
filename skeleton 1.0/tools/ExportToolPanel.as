package 
{
	import akdcl.skeleton.Armature;
	import akdcl.skeleton.ArmatureAniData;
	import akdcl.skeleton.BaseCommand;
	import akdcl.skeleton.ConnectionData;
	
	import com.grapefrukt.exporter.simple.SimpleExport;
	import com.grapefrukt.exporter.extractors.*;
	
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Bitmap;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.events.MouseEvent;
	import flash.utils.setTimeout;

	import flash.net.SharedObject;
	import flash.net.FileReference;
	
	import flash.utils.getDefinitionByName;
	import flash.utils.getQualifiedClassName;

	import fl.containers.ScrollPane;
	import fl.controls.Button;
	import fl.controls.NumericStepper;
	import fl.controls.Slider;
	import fl.controls.ComboBox;
	

	import PNGEncoder;

	import XMLToObject;

	import akdcl.skeleton.export.TexturePacker;
	import akdcl.skeleton.export.TextureMix;
	import akdcl.skeleton.export.ConnectionEncoder;
	import akdcl.skeleton.export.ContourInstaller;
	import starling.display.Button;



	/**
	 * ExportFLAClass
	 * @author Akdcl
	 */
	public dynamic class ExportToolPanel extends Sprite {;
	public var paneTextureImage:ScrollPane;

	public var comboBoxArmature:ComboBox;
	public var comboBoxAnimation:ComboBox;
	public var comboBoxTextureSize:ComboBox;
	public var comboBoxDataType:ComboBox;

	public var numEase:NumericStepper;
	public var sliderAnimationSpeed:Slider;

	public var btnTextureSave:Button;
	public var btnSkeletonSave:Button;
	public var btnTextureSaveAsSingleFile:Button;
	
	public var export:SimpleExport;

	private var swfName:String;

	private var texturePacker:TexturePacker;
	private var skeletons:XML;
	private var textureMix:TextureMix;
	private var armatures:Array;
	private var armatureShow:Armature;

	private var bmp:Bitmap;
	private var fileREF:FileReference;
	private var shareObject:SharedObject;

	public function ExportToolPanel()
	{
		armatures = [];
		skeletons =<root/>;
		texturePacker = new TexturePacker  ;
		bmp = new Bitmap  ;
		fileREF = new FileReference  ;
		fileREF.addEventListener(Event.CANCEL,onImageCancleHandler);
		shareObject = SharedObject.getLocal("akdcl/skeleton/ExportToolPanel","/");
		addEventListener(Event.ADDED_TO_STAGE,onAddToStageHandler);

		if (root)
		{

			var _child:MovieClip;
			var _root:DisplayObjectContainer = root as DisplayObjectContainer;
			trace(_root.name);
			for (var _i:uint = 0; _i < _root.numChildren; _i++)
			{
				_child = _root.getChildAt(_i) as MovieClip;

				if (_child)
				{
					trace(_child.name);
					_child.gotoAndStop(1);

				}
			}
		}
	}

	private function onAddToStageHandler(e:Event):void
	{
		removeEventListener(Event.ADDED_TO_STAGE,onAddToStageHandler);
		x = 0;
		y = 0;

		swfName = decodeURI(this.loaderInfo.url);
		trace(swfName);
		swfName = swfName.split("/").pop();
		swfName = swfName.split("\\").pop();
		swfName = swfName.split(".").shift();

		paneTextureImage.source = bmp;

		comboBoxTextureSize.addItem({label:"128"});
		comboBoxTextureSize.addItem({label:"256"});
		comboBoxTextureSize.addItem({label:"512"});
		comboBoxTextureSize.addItem({label:"1024"});
		comboBoxTextureSize.addItem({label:"2048"});

		comboBoxDataType.addItem({label:"XML"});
		comboBoxDataType.addItem({label:"JSON"});

		btnTextureSave.label = "保存贴图数据";
		btnSkeletonSave.label = "保存骨骼数据";
		btnTextureSaveAsSingleFile.label = "保存贴图数据作为单个文件";

		comboBoxTextureSize.selectedIndex = shareObject.data["textureSize"] || 2;
		comboBoxDataType.selectedIndex = shareObject.data["dataType"] || 0;

		sliderAnimationSpeed.liveDragging = true;
		sliderAnimationSpeed.maximum = 4;
		sliderAnimationSpeed.minimum = 0.1;
		sliderAnimationSpeed.snapInterval = 0.1;
		sliderAnimationSpeed.value = 1;

		numEase.maximum = 2;
		numEase.minimum = -1;
		numEase.value = 2;

		comboBoxArmature.addEventListener(Event.CHANGE,onComponentsChangeHandler);
		comboBoxAnimation.addEventListener(Event.CHANGE,onComponentsChangeHandler);
		comboBoxTextureSize.addEventListener(Event.CHANGE,onComponentsChangeHandler);
		comboBoxDataType.addEventListener(Event.CHANGE,onComponentsChangeHandler);

		btnTextureSave.addEventListener(MouseEvent.CLICK,onBtnClickHandler);
		btnSkeletonSave.addEventListener(MouseEvent.CLICK,onBtnClickHandler);
		btnTextureSaveAsSingleFile.addEventListener(MouseEvent.CLICK, onBtnClickHandler);

		sliderAnimationSpeed.addEventListener(Event.CHANGE,onComponentsChangeHandler);
		numEase.addEventListener(Event.CHANGE,onComponentsChangeHandler);

		setTimeout(searchAllClip,20);
		addEventListener(Event.ENTER_FRAME,onEnterFrameHandler);
	}

	private function onBtnClickHandler(e:MouseEvent):void
	{
		if (armatures.length == 0)
		{
			return;
		}
		switch (e.target)
		{
			case btnTextureSave :
				fileREF.addEventListener(Event.COMPLETE,onImageCompleteHandler);
				fileREF.save(PNGEncoder.encode(textureMix.bitmapData),swfName + ".png");
				break;
			case btnTextureSaveAsSingleFile :
				/*
				var sheet:DisplayObjectContainer = (root as DisplayObjectContainer).getChildByName(comboBoxArmature.selectedLabel); 
				export.textures.add(TextureExtractor.extract( sheet ));
				export.export();*/
				export.output();
				break;
			case btnSkeletonSave :
				switch (comboBoxDataType.selectedLabel)
				{
					case "XML" :
						fileREF.save(skeletons,swfName + "_skeleton.xml");
						break;
					case "JSON" :
						var _jsonStr:String = JSON.stringify(new XMLToObject(skeletons).data);
						_jsonStr = "skeleton.addSkeletonData(" + _jsonStr + ");";
						fileREF.save(_jsonStr,swfName + "_skeleton.js");
						break;
				}
				break;
			
			
		}
	}

	private function onComponentsChangeHandler(e:Event):void
	{
		switch (e.target)
		{
			case comboBoxTextureSize :
				packTextures();
				shareObject.data["textureSize"] = comboBoxTextureSize.selectedIndex;
				break;
			case comboBoxDataType :
				shareObject.data["dataType"] = comboBoxDataType.selectedIndex;
				break;
			case comboBoxArmature :
				changeArmature(armatures[comboBoxArmature.selectedIndex]);
				break;
			case comboBoxAnimation :
				changeAnimation(comboBoxAnimation.selectedLabel);
				break;
			case sliderAnimationSpeed :
				if (armatureShow)
				{
					armatureShow.animation.setAnimationScale(sliderAnimationSpeed.value);
				}
				break;
			case numEase :
				if (armatureShow)
				{
					changeAnimation(comboBoxAnimation.selectedLabel);
				}
				break;
		}
	}

	private function onImageCompleteHandler(e:Event):void
	{
		fileREF.removeEventListener(Event.COMPLETE,onImageCompleteHandler);
		switch (comboBoxDataType.selectedLabel)
		{
			case "XML" :
				fileREF.save(textureMix.xml,swfName + "_texture.xml");
				break;
			case "JSON" :
				var _jsonStr:String = JSON.stringify(new XMLToObject(textureMix.xml).data);
				_jsonStr = "skeleton.addTextureData(" + _jsonStr + ");";
				fileREF.save(_jsonStr,swfName + "_texture.js");
				break;
		}
	}

	private function onImageCancleHandler(e:Event):void
	{
		fileREF.removeEventListener(Event.COMPLETE,onImageCompleteHandler);
	}

	private function searchAllClip():void
	{
		var _child:MovieClip;
		var _root:DisplayObjectContainer = root as DisplayObjectContainer;
		var _xml:XML;
		var _id:String;
		
		export = new SimpleExport(_root);
		
		var _childTexture:MovieClip ;
		
		for (var _i:uint = 0; _i < _root.numChildren; _i++)
		{
			var _newChild:* = getDefinitionByName(getQualifiedClassName (_root.getChildAt(_i) ));
			export.textures.add( TextureExtractor.extract( new _newChild() ) );
			
		}
		for (var _i:uint = 0; _i < _root.numChildren; _i++)
		{
			_child = ContourInstaller.install(_root.getChildAt(_i) as MovieClip);
			if (_child)
			{
				_child.visible = false;
				_child.reset();
				_id = _child.getName();
				_xml = ConnectionEncoder.encode(_child);
				//trace(_xml);
				skeletons.appendChild(_xml.copy());
				ConnectionData.addData(_xml);

				texturePacker.addTexturesFromContainer(_child,_id);
				armatures.push(BaseCommand.createArmature(_id,_id,_child,null,null,true));
				comboBoxArmature.addItem({label:_id});
			}
		}
		
		export.export();

		//trace("骨骼动画数据\n" + skeletons);
		packTextures();

		changeArmature(armatures[0]);
	}

	private function packTextures():void
	{
		if (armatures.length == 0)
		{
			return;
		}
		if (textureMix)
		{
			textureMix.dispose();
		}
		textureMix = texturePacker.packTextures(int(comboBoxTextureSize.selectedLabel),2);
		bmp.bitmapData = textureMix.bitmapData;
		paneTextureImage.source = bmp;
	}

	private function changeArmature(_armature:Armature):void
	{
		if (armatureShow)
		{
			armatureShow.animation.stop();
			armatureShow.getDisplay().visible = false;
		}
		armatureShow = _armature;
		armatureShow.getDisplay().visible = true;
		armatureShow.getDisplay().x = 660;
		armatureShow.getDisplay().y = 150;
		addChild(armatureShow.getDisplay() as MovieClip);

		var _animations:Array = armatureShow.animation.getData().getAnimationNames();
		comboBoxAnimation.removeAll();

		for each (var _name:String in _animations)
		{
			comboBoxAnimation.addItem({label:_name});
		}
		changeAnimation(_animations[0]);
	}

	private function changeAnimation(_animation:String):void
	{
		armatureShow.animation.playTo(_animation,10,30,true,numEase.value);
	}

	private function onEnterFrameHandler(e:Event):void
	{
		if (armatureShow)
		{
			armatureShow.update();
		}
	}
}
}