/**
 * 骨骼动画V1
 * @author ...akdcl
 */
var skeleton = {};
(function(){

var PI = Math.PI,
	HALF_PI = PI * 0.5,
	ANGLE_TO_RADIAN = PI / 180;
	
var ptp;

/**
 * Node
 */
skeleton.Node = function(x, y, rotation){
	this.x = x || 0;
	this.y = y || 0;
	this.rotation = rotation || 0;
}

ptp = skeleton.Node.prototype;
ptp.copy = function(node){
	this.x = node.x;
	this.y = node.y;
	this.rotation = node.rotation;
}

/**
 * FrameNode
 */
skeleton.FrameNode = function(x, y, rotation){
	skeleton.Node.call(this, x, y, rotation);
	this.scaleX = 1;
	this.scaleY = 1;
	this.alpha = 1;
	this.frame = 1;
	this.offR = 0;
}
skeleton.FrameNode.prototype = new skeleton.Node();

ptp = skeleton.FrameNode.prototype;
ptp.copy = function(node){
	this.constructor.prototype.copy.call(this, node);
	if(node.frame){
		this.scaleX = node.scaleX;
		this.scaleY = node.scaleY;
		this.alpha = node.alpha;
		this.frame = node.frame;
		this.offR = node.offR;
	}
}

/**
 * TweenNode
 */
skeleton.TweenNode = function(x, y, rotation){
	skeleton.FrameNode.call(this, x, y, rotation);
	this.sR = 0;
	this.sX = 0;
	this.sY = 0;
	this.sSX = 0;
	this.sSY = 0;
	this.sA = 0;
	
	this.dR = 0;
	this.dX = 0;
	this.dY = 0;
	this.dSX = 0;
	this.dSY = 0;
	this.dA = 0;
}
skeleton.TweenNode.prototype = new skeleton.FrameNode();

ptp = skeleton.TweenNode.prototype;
ptp.betweenValue = function(from, to){
	this.sR = from.rotation;
	this.sX = from.x;
	this.sY = from.y;
	this.sSX = from.scaleX;
	this.sSY = from.scaleY;
	this.sA = from.alpha;
	if(to.offR){
		this.dR = to.rotation + to.offR * 360 - this.sR;
	}else{
		this.dR = to.rotation - this.sR;
	}
	this.dX = to.x - this.sX;
	this.dY = to.y - this.sY;
	this.dSX = to.scaleX - this.sSX;
	this.dSY = to.scaleY - this.sSY;
	this.dA = to.alpha - this.sA;
}
ptp.tweenTo = function(k){
	this.rotation = this.sR + this.dR * k;
	this.x = this.sX + this.dX * k;
	this.y = this.sY + this.dY * k;
	
	if(this.dSX){
		this.scaleX = this.sSX + this.dSX * k;
	}else{
		this.scaleX = NaN;
	}
	if(this.dSY){
		this.scaleY = this.sSY + this.dSY * k;
	}else{
		this.scaleY = NaN;
	}
	if(this.dA){
		this.alpha = this.sA + this.dA * k;
	}else{
		this.alpha = NaN;
	}
}

/**
 * FrameNodeList
 */
skeleton.FrameNodeList = function(delay, scale){
	this.delay = delay || 0;
	this.scale = scale || 1;
	this.frame = 0;
	this.length = 0;
	this.frameList = [];
}

ptp = skeleton.FrameNodeList.prototype;
ptp.addFrame = function(node){
	this.frameList.push(node);
	this.frame += node.frame;
	this.length ++;
}
ptp.getFrame = function(index){
	if(index < 0){
		index = this.length + index;
	}
	return this.frameList[index];
}

/**
 * ProcessBase
 */
skeleton.ProcessBase = function(){
	this.scale = 1;
	this.isComplete = true;
	this.isPause = false;
	this.currentFrame = 0;
	
	this.totalFrames = 0;
	this.listFrames = 0;
	this.noScaleListFrames = 0;
	this.currentPrecent = 0;
	this.loop = 0;
	this.ease = 0;
	this.toFrameID = 0;
	this.betweenFrame = 0;
	this.listEndFrame = 0;
}

ptp = skeleton.ProcessBase.prototype;
ptp.remove = function(){
	this.scale = 1;
	this.isComplete = true;
	this.isPause = false;
	this.currentFrame = 0;
}
ptp.pause = function(){
	this.isPause = true;
}
ptp.resume = function(){
	this.isPause = false;
}
ptp.stop = function(){
	this.isComplete = true;
	this.currentFrame = 0;
}
ptp.playTo = function(to, toFrames, listFrames, isLoop, ease){
	this.isComplete = false;
	this.isPause = false;
	this.currentFrame = 0;
	this.totalFrames = toFrames;
	this.ease = ease || 0;
}
ptp.update = function(){
	if(this.isComplete || this.isPause){
		return;
	}
	this.currentFrame += this.scale;
	this.currentPrecent = this.currentFrame / this.totalFrames;
	this.currentFrame %= this.totalFrames;
	this.updateHandler();
}

/**
 * Tween
 */
skeleton.Tween = function(){
	this.from = new skeleton.TweenNode();
	this.to = new skeleton.TweenNode();
	//node:TweenNode;
	//tweenList:FrameNodeList;
}
skeleton.Tween.prototype = new skeleton.ProcessBase();

skeleton.Tween.prepared = [];
skeleton.Tween.create = function(){
	if(skeleton.Tween.prepared.length > 0){
		return skeleton.Tween.prepared.pop();
	}
	return new skeleton.Tween();
}
skeleton.Tween.recycle = function(tween){
	if(skeleton.Tween.prepared.indexOf(tween) <0 ){
		return;
	}
	skeleton.Tween.prepared.push(tween);
}

ptp = skeleton.Tween.prototype;
ptp.remove=function(){
	this.constructor.prototype.remove.call(this);
	this.node = null;
	this.tweenList = null;
	skeleton.Tween.recycle(this);
}
ptp.setNode = function(node){
	this.node = node;
}
ptp.playTo = function(to, toFrames, listFrames, isLoop, ease){
	this.constructor.prototype.playTo.call(this, to, toFrames, listFrames, isLoop, ease);
	this.node.rotation %= 360;
	this.from.copy(this.node);
	this.tweenList = to;
	if(to.length == 1){
		this.loop = -4;
		this.to.copy(this.tweenList.getFrame(0));
	}else{
		if(isLoop){
			this.loop = -2;
			this.noScaleListFrames = this.tweenList.frame;
		}else{
			this.loop = -3;
			this.noScaleListFrames = this.tweenList.frame - 1;
		}
		this.listFrames = listFrames * this.tweenList.scale;
		if(isLoop && this.tweenList.delay != 0){
			var playedFrames = this.noScaleListFrames * (1 - this.tweenList.delay);
			var prevFrameID = 0;
			var toFrameID = 0;
			var listEndFrame = 0;
			var betweenFrame = 0;
			do{
				betweenFrame = this.tweenList.getFrame(toFrameID).frame;
				listEndFrame += betweenFrame;
				prevFrameID = toFrameID;
				if(++toFrameID >= this.tweenList.length){
					toFrameID = 0;
				}
			}while(playedFrames >= listEndFrame);
			
			this.to.betweenValue(this.tweenList.getFrame(prevFrameID), this.tweenList.getFrame(toFrameID));
			var currentPrecent = 1 - (listEndFrame - playedFrames) / betweenFrame;
			if(this.ease == 2){
				currentPrecent = 0.5 * (1 - Math.cos(currentPrecent * Math.PI ));
			}else if(this.ease != 0){
				currentPrecent = this.ease > 0?Math.sin(currentPrecent * HALF_PI):(1 - Math.cos(currentPrecent * HALF_PI));
			}
			this.to.tweenTo(currentPrecent);
		}else {
			this.to.copy(this.tweenList.getFrame(0));
		}
	}
	this.node.betweenValue(this.from, this.to);
}
ptp.updateHandler = function(){
	if(this.currentPrecent >= 1){
		switch(this.loop){
			case -3:
				this.loop = -1;
				this.currentPrecent = (this.currentPrecent - 1) * this.totalFrames / this.listFrames;
				if(this.currentPrecent >= 1){
				}else{
					this.totalFrames = this.listFrames;
					this.currentPrecent %= 1;
					this.listEndFrame = 0;
					break;
				}
			case -1:
			case -4:
				this.currentPrecent = 1;
				this.isComplete = true;
				break;
			case -2:
				this.loop = 0;
				this.totalFrames = this.listFrames;
				if(this.tweenList.delay != 0){
					this.currentFrame = (1 - this.tweenList.delay) * this.totalFrames;
					this.currentPrecent += this.currentFrame / this.totalFrames;
				}
				this.currentPrecent %= 1;
				this.listEndFrame = 0;
				break;
			default:
				this.loop += this.currentPrecent >> 0;
				this.currentPrecent %= 1;
				break;
		}
	}else if(this.loop < -1){
		this.currentPrecent = Math.sin(this.currentPrecent * HALF_PI);
	}
	
	if(this.loop >= -1){
		this.updateCurrentPrecent();
	}
	this.node.tweenTo(this.currentPrecent);
}

ptp.updateCurrentPrecent = function(){
	var playedFrames = this.noScaleListFrames * this.currentPrecent;
	if(playedFrames <= this.listEndFrame-this.betweenFrame || playedFrames > this.listEndFrame){
		this.listEndFrame = 0;
		this.toFrameID = 0;
		var prevFrameID = 0;
		do{
			this.betweenFrame = this.tweenList.getFrame(this.toFrameID).frame;
			this.listEndFrame += this.betweenFrame;
			prevFrameID = this.toFrameID;
			if(++this.toFrameID >= this.tweenList.length){
				this.toFrameID = 0;
			}
		}while(playedFrames >= this.listEndFrame);
		
		this.from.copy(this.tweenList.getFrame(prevFrameID));
		this.to.copy(this.tweenList.getFrame(this.toFrameID));
		this.node.betweenValue(this.from, this.to);
	}
	
	this.currentPrecent = 1 - (this.listEndFrame - playedFrames) / this.betweenFrame;
	if(this.ease == 2){
		this.currentPrecent = 0.5 * (1 - Math.cos(this.currentPrecent * PI));
	}else if(this.ease != 0){
		this.currentPrecent = this.ease > 0?Math.sin(this.currentPrecent * HALF_PI):(1 - Math.cos(this.currentPrecent * HALF_PI));
	}
}

/**
 * Animation
 */
skeleton.Animation = function(){
	//tweenDic:Object
	//armatureAniData:Object
	//boneAniData:Object
	//aniIDNow:String
	//onAnimation:Function
	//eventTarget
}
skeleton.Animation.prototype = new skeleton.ProcessBase();

ptp = skeleton.Animation.prototype;
ptp.remove = function(){
	this.constructor.prototype.remove.call(this);
	for(var boneName in this.tweenDic){
		this.tweenDic[boneName].remove();
	}
	this.tweenDic = null;
	this.armatureAniData = null;
	this.boneAniData = null;
	this.aniIDNow = null;
}
ptp.pause = function(){
	this.constructor.prototype.pause.call(this);
	for(var boneName in this.tweenDic){
		this.tweenDic[boneName].pause();
	}
}
ptp.resume = function(){
	this.constructor.prototype.resume.call(this);
	for(var boneName in this.tweenDic){
		this.tweenDic[boneName].resume();
	}
}
ptp.stop = function(){
	this.constructor.prototype.stop.call(this);
	for(var boneName in this.tweenDic){
		this.tweenDic[boneName].stop();
	}
}
ptp.setData = function(aniData){
	this.remove();
	this.tweenDic = {};
	this.armatureAniData = aniData;
}
ptp.addTween = function(bone){
	var boneName = bone.name;
	var tween = this.tweenDic[boneName];
	if(!tween){
		this.tweenDic[boneName] = tween = skeleton.Tween.create();
	}
	tween.setNode(bone.tweenNode);
}
ptp.removeTween = function(bone){
	var boneName = bone.name;
	var tween = this.tweenDic[boneName];
	if(tween){
		delete this.tweenDic[boneName];
		tween.remove();
	}
}
ptp.getTween = function(boneName){
	return this.tweenDic[boneName];
}
ptp.updateTween = function(boneName){
	var tween = this.tweenDic[boneName];
	if(tween){
		tween.update();
	}
}
ptp.playTo = function(to, toFrames, listFrames, isLoop, ease){
	this.boneAniData = this.armatureAniData[to];
	if(!this.boneAniData){
		return;
	}
	this.constructor.prototype.playTo.call(this, to, toFrames, listFrames, isLoop, ease);
	this.aniIDNow = to;
	var frameNodeList;
	var tween;
	for(var boneName in this.tweenDic){
		tween = this.tweenDic[boneName];
		frameNodeList = this.boneAniData[boneName];
		if(frameNodeList){
			tween.playTo(frameNodeList, toFrames, listFrames, isLoop, ease);
		}
	}
	this.noScaleListFrames = this.boneAniData.frame;
	if(this.noScaleListFrames == 1){
		this.loop = -4;
	}else{
		if(isLoop){
			this.loop = -2;
		}else{
			this.loop = -3;
			this.noScaleListFrames --;
		}
		this.listFrames = listFrames;
	}
}
ptp.updateHandler = function(){
	if(this.currentPrecent >= 1){
		switch(this.loop){
			case -3:
				this.loop = -1;
				this.currentPrecent = (this.currentPrecent - 1) * this.totalFrames / this.listFrames;
				if(this.currentPrecent >= 1){
				}else{
					this.currentPrecent %= 1;
					this.totalFrames = this.listFrames;
					this.listEndFrame = 0;
					if(this.onAnimation != null){
						this.onAnimation.call(this.eventTarget, "start", this.aniIDNow);
					}
					break;
				}
			case -1:
			case -4:
				this.currentPrecent = 1;
				this.isComplete = true;
				if(this.onAnimation != null){
					this.onAnimation.call(this.eventTarget, "complete", this.aniIDNow);
				}
				break;
			case -2:
				this.loop = 0;
				this.currentPrecent %= 1;
				this.totalFrames = this.listFrames;
				this.listEndFrame = 0;
				if(this.onAnimation != null){
					this.onAnimation.call(this.eventTarget, "start", this.aniIDNow);
				}
				break;
			default:
				this.loop += this.currentPrecent >> 0;
				this.currentPrecent %= 1;
				if(this.onAnimation != null){
					this.onAnimation.call(this.eventTarget, "loopComplete", this.aniIDNow);
				}
				break;
		}
	}
	if(this.loop >= -1 && this.boneAniData.eventFrame){
		this.updateCurrentPrecent();
	}
}
ptp.updateCurrentPrecent = function(){
	var playedFrames = this.noScaleListFrames * this.currentPrecent;
	if(playedFrames <= this.listEndFrame - this.betweenFrame || playedFrames > this.listEndFrame){
		this.toFrameID = 0;
		this.listEndFrame = 0;
		var prevFrameID;
		do{
			this.betweenFrame = this.boneAniData.eventFrame[this.toFrameID].frame;
			this.listEndFrame += this.betweenFrame;
			prevFrameID = this.toFrameID;
			if(++ this.toFrameID >= this.boneAniData.eventFrame.length){
				this.toFrameID = 0;
			}
		}while(playedFrames >= this.listEndFrame);
		if(this.onAnimation != null){
			this.onAnimation.call(this.eventTarget, "inFrame", this.aniIDNow, this.boneAniData.eventFrame[prevFrameID].name);
		}
	}	
	this.currentPrecent = 1 - (this.listEndFrame - playedFrames) / this.betweenFrame;
}
ptp.setAnimationScale = function(scale, boneName){
	var tween;
	if(boneName){
		tween = this.tweenDic[boneName];
		if(tween){
			tween.scale = scale;
		}
	}else{
		this.scale = scale;
		for(var boneName in this.tweenDic){
			this.tweenDic[boneName].scale = scale;
		}
	}
}

/**
 * Bone
 */
skeleton.Bone=function(){
	this.transformX = 0;
	this.transformY = 0;
	this.parentX = 0;
	this.parentY = 0;
	this.parentR = 0;
	this.lockX = 0;
	this.lockY = 0;
	this.lockR = 0;
	
	this.node=new skeleton.Node();
	this.tweenNode=new skeleton.TweenNode();
	//this.name;
	//this.display;
	//this.parent;
	
}
skeleton.Bone.prepared = [];
skeleton.Bone.create = function(){
	if(skeleton.Bone.prepared.length > 0){
		return skeleton.Bone.prepared.pop();
	}
	return new skeleton.Bone();
}
skeleton.Bone.recycle = function(bone){
	if(skeleton.Bone.prepared.indexOf() < 0){
		return;
	}
	skeleton.Bone.prepared.push(bone);
}

ptp = skeleton.Bone.prototype;
ptp.remove = function(){
	this.display = null;
	this.name = null;
	this.parent = null;
	this.transformX = 0;
	this.transformY = 0;
	this.parentX = 0;
	this.parentY = 0;
	this.parentR = 0;
	this.lockX = 0;
	this.lockY = 0;
	this.lockR = 0;
	skeleton.Bone.recycle(this);
}
ptp.getGlobalX = function(){
	return this.transformX + this.parentX;
}
ptp.getGlobalY = function(){
	return this.transformY + this.parentY;
}
ptp.getGlobalR = function(){
	return this.node.rotation + this.tweenNode.rotation + this.parentR + this.lockR;
}
ptp.addChild = function(child){
	child.parent = this;
	return child;
}
ptp.setLockPosition=function(x, y, r){
	this.lockX = x;
	this.lockY = y;
	this.lockR = r || 0;
}
ptp.update = function(){
	if(this.parent){
		this.parentX = this.parent.getGlobalX();
		this.parentY = this.parent.getGlobalY();
		this.parentR = this.parent.getGlobalR();
		
		var _dX = this.lockX + this.node.x + this.tweenNode.x;
		var _dY = this.lockY + this.node.y + this.tweenNode.y;
		var _r = Math.atan2(_dY, _dX) + this.parentR * ANGLE_TO_RADIAN;
		var _len = Math.sqrt(_dX * _dX + _dY * _dY);
		this.transformX = _len * Math.cos(_r);
		this.transformY = _len * Math.sin(_r);
	}else{
		this.transformX = this.node.x + this.tweenNode.x;
		this.transformY = this.node.y + this.tweenNode.y;
	}
	this.updateDisplay();
}
ptp.updateDisplay = function(){
	if(this.display){
		this.display.x = this.transformX + this.parentX;
		this.display.y = this.transformY + this.parentY;
		var rotation = this.node.rotation + this.tweenNode.rotation + this.parentR + this.lockR;
		rotation%=360;
		if(rotation<0){
			rotation+=360;
		}
		this.display.rotation = rotation;
		
		if(isNaN(this.tweenNode.scaleX)){
		}else{
			this.display.scaleX = this.tweenNode.scaleX;
		}
		if(isNaN(this.tweenNode.scaleY)){
		}else{
			this.display.scaleY = this.tweenNode.scaleY;
		}
		if(isNaN(this.tweenNode.alpha)){
		}else{
			if(this.tweenNode.alpha){
				this.display.visible = true;
				this.display.alpha = this.tweenNode.alpha;
			}else{
				this.display.visible = false;
			}
		}
	}
}

/**
 * Armature
 */
skeleton.Armature = function(display){
	this.boneDic = {};
	this.boneList = [];
	this.display = display;
	this.animation = new skeleton.Animation();
	//this.name;
}

ptp = skeleton.Armature.prototype;
ptp.update=function(){
	var len = this.boneList.length;
	var bone;
	for(var index = 0; index < len; index++){
		bone = this.boneList[index];
		this.animation.updateTween(bone.name);
		bone.update();
	}
	this.animation.update();
}
ptp.remove = function(){
	for(var index in this.boneList){
		this.boneList[index].remove();
	}
	this.animation.remove();
	this.animation = null;
	this.display = null;
	this.boneDic = null;
	this.boneList = null;
}
ptp.addBone = function(name, parentName, display, index){
	var bone = this.boneDic[name];
	if(!bone){
		bone = skeleton.Bone.create();
		bone.name = name;
		this.boneList.push(bone);
		this.boneDic[name] = bone;
		var boneParent = this.boneDic[parentName];
		if(boneParent){
			boneParent.addChild(bone);
		}
		this.animation.addTween(bone);
	}
	
	if(display){
		if(display.name != name){
			display.name = name;
		}
		var displayOld = bone.display;
		bone.display = display;
		if(displayOld){
			this.display.addChildAt(display, this.display.getChildIndex(displayOld) - 1);
		}else if(index < 0){
			this.display.addChild(display);
		}else{
			this.display.addChildAt(display, index);
		}
	}
	return bone;
}
ptp.removeBone = function(name){
	var bone = this.boneDic[name];
	if(bone){
		if(bone.display && this.display.contains(bone.display)){
			this.display.removeChild(bone.display);
		}
		this.animation.removeTween(bone);
		bone.remove();
	}
}
ptp.getBone = function(name){
	return this.boneDic[name];
}
ptp.getDisplay = function(){
	return this.display;
}

skeleton.textureDatas = {};
skeleton.armarureDatas = {};
skeleton.animationDatas = {};

skeleton.getTextureDisplay = function(image, fullName){
	var textureData = skeleton.getTextureData(fullName);
	if(textureData){
		var display = new Bitmap(image, [textureData.x, textureData.y, textureData.width, textureData.height]);
		display.regX = -textureData.frameX;
		display.regY = -textureData.frameY;
		return display;
	}
	return null;
}

skeleton.addTextureData = function (json){
	var textureData;
	json.SubTexture = skeleton.fixArray(json.SubTexture);
	for(var index in json.SubTexture){
		textureData = json.SubTexture[index];
		skeleton.textureDatas[textureData.name] = textureData;
		json.SubTexture[index] = null;
	}
}

skeleton.getTextureData = function(fullName){
	return skeleton.textureDatas[fullName];
}

skeleton.getTextureList = function(prefix){
	var list = [];
	for(var fullName in skeleton.textureDatas){
		if(fullName.indexOf(prefix + "_") == 0){
			list[skeleton.getTextureSuffix(fullName, prefix)] = skeleton.textureDatas[fullName];
		}
	}
	return list;
}

skeleton.getTexturePreFix = function(fullName){
	var arr = fullName.split("_");
	return arr.length > 1?arr[0]:null;
}

skeleton.getTextureSuffix = function(fullName, prefix){
	if(!prefix){
		prefix = skeleton.getTexturePreFix(fullName);
	}
	if(prefix){
		prefix += "_";
		var index = fullName.indexOf(prefix);
		if(index == 0){
			return fullName.substr(index + prefix.length);
		}
	}
	return null;
}

skeleton.addSkeletonData = function(json){
	json.skeleton = skeleton.fixArray(json.skeleton);
	for(var index in json.skeleton){
		skeleton._addSkeletonData(json.skeleton[index]);
	}
}

skeleton._addSkeletonData = function(skeletonData){
	var name = skeletonData.name;
	var aniData = skeleton.animationDatas[name];
	if(aniData){
		return;
	}
	skeleton.animationDatas[name] = aniData = {};
	var eachMovement,
		eachBoneAni,
		animationList = skeleton.fixArray(skeletonData.animation);
	
	for(var index in animationList){
		eachMovement = animationList[index];
		animationList[index]=null;
		aniData[eachMovement.name] = eachMovement;
		for(var boneName in eachMovement){
			eachBoneAni = eachMovement[boneName];
			if(typeof(eachBoneAni) == "object" && boneName != "eventFrame"){
				eachBoneAni = skeleton.fixArray(eachBoneAni);
				eachMovement[boneName] = skeleton._getFrameNodeList(eachBoneAni);
			}
		}
		
		if(eachMovement.eventFrame){
			eachMovement.eventFrame = skeleton.fixArray(eachMovement.eventFrame);
			var _obj = {};
			var _frame = 0;
			for(var i in eachMovement.eventFrame){
				_frame += eachMovement.eventFrame[i].frame;
			}
			_obj.name = "init";
			_obj.frame = eachMovement.frame - _frame;
			eachMovement.eventFrame.unshift(_obj);
		}
	}
	
	skeleton.armarureDatas[name] = skeleton.fixArray(skeletonData.bone);
	
	delete skeletonData.animation;
	delete skeletonData.bone;
}

skeleton._getFrameNodeList=function(boneAni){
	var nodeList = new skeleton.FrameNodeList();
	nodeList.scale = boneAni[0].scale || nodeList.scale;
	nodeList.delay = boneAni[0].delay || nodeList.delay;
	nodeList.frameList = boneAni;
	nodeList.length = boneAni.length;
	
	var node;
	for(var index in boneAni){
		node=boneAni[index];
		node.rotation = Number(node.rotation) || 0;
		node.scaleX = Number(node.scaleX) || 1;
		node.scaleY = Number(node.scaleY) || 1;
		node.alpha = Number(node.alpha) || 1;
		node.offR = Number(node.offR) || 0;
		node.frame = Number(node.frame) || 1;
		nodeList.frame += node.frame;
	}
	return nodeList;
}

skeleton.getArmatureData = function(name){
	return skeleton.armarureDatas[name];
}

skeleton.getAnimationData = function(name){
	return skeleton.animationDatas[name];
}

skeleton.createArmature = function(name, animationName, image){
	var armatureData = skeleton.getArmatureData(name);
	if(!armatureData){
		return null;
	}
	var armatureDisplay = new Sprite();
	armatureDisplay.name = name;
	var armature = new skeleton.Armature(armatureDisplay);
	
	var animationData = skeleton.getAnimationData(animationName);
	if(animationData){
		armature.animation.setData(animationData);
	}
	
	var bone,
		boneData,
		boneName,
		parentName,
		boneDisplay,
		displayHigher,
		indexZ,
		list = [],
		length = armatureData.length;
	for(var indexI = 0; indexI < length; indexI++){
		boneData = armatureData[indexI];
		boneName = boneData.name;
		parentName = boneData.parent;
		indexZ = boneData.z;
		
		
		boneDisplay = skeleton.getTextureDisplay(image, name + "_" + boneName);
		if(boneDisplay){
			displayHigher = null;
			for(var indexJ = indexZ; indexJ < list.length; indexJ++){
				displayHigher = list[indexJ];
				if(displayHigher){
					break;
				}
			}
			list[indexZ] = boneDisplay;
			if(displayHigher){
				indexZ = armature.display.getChildIndex(displayHigher);
			}else{
				indexZ = -1;
			}
		}
		
		bone = armature.addBone(boneName, parentName, boneDisplay, indexZ);
		bone.setLockPosition(boneData.x, boneData.y, 0);
	}
	return armature;
}
skeleton.fixArray = function(arr){
	if(arr && !(arr instanceof Array)){
		return [arr];
	}
	return arr;
}

ptp = null;
})();