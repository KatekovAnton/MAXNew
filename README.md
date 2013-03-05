# Getting Started with the Seedr iOS SDK

The Seedr iOS SDK is designed to be easy to install and use. This guide walks you through the basics of setting up the development and project environment and creating a simple app.

<br>
###  Step 1. Install the Prerequisites

You need [Xcode](https://developer.apple.com/xcode/), and optionally, [Git](http://git-scm.com/)

####  Note
> OS X is required for all iOS development. If you're completely new to iOS development, you'll need Xcode installed on your Mac too. It's available through the [App Store](https://itunes.apple.com/us/app/xcode/id497799835?ls=1&mt=12)

<br>
###  Step 2. Install the Seedr iOS SDK

[Download](http://www.google.com) the Seedr SDK for iOS to any folder you wish. <br>

####  Git command
>git clone git@github.com:burovgleb/seedr.ios.samples.git


####  Note
> Source code for the SDK is not available

<br>
###  Step 3. Run the Samples

Try the sample project available on GitHub demonstrates a technique for building apps.

####  Git command
>git clone git@github.com:burovgleb/seedr.ios.samples.git

<br>
###  Step 4. Create a Seedr App

Coming soon.

####  Note
>For most actual information see [Seedr oficaial web page](http://seedr.ru/)

<br>
###  Step 4. Configure a your Project

You need to add Seedr library and its header files to your project to get acces to Seedr. 
First you need is open Project Navigator in your XCode project and open Seedr SDK folder with Finder.

![](https://s3-eu-west-1.amazonaws.com/static.seedr.ru/ios-step-1.png)


Add the Seedr SDK for iOS into your project by dragging the Seedr SDK folder from Finder into the Frameworks section of your Project Navigator.

![](https://s3-eu-west-1.amazonaws.com/static.seedr.ru/ios-step-2.png)

Choose 'Create groups for any added folders'. There is no matter for you in enable or disable 'Copy items into destination group's folder (if needed)'-option - if it is selected, Xcode will keep the reference to the SDK installation folder. If not - it will means Xcode will create a copy.

![](https://s3-eu-west-1.amazonaws.com/static.seedr.ru/ios-step-3.png)

<br>
###  Step 5. Initialize Seedr


Incorporate the following two lines of Seedr code: 

<!-- language: objective-c -->
	#import "Seedr.h"
	- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
	{	
		[[Seedr instance] startSessionWithApiKey:@"YOUR_API_KEY"];
		// Pointer to your rootViewController
		[[Seedr instance] initialize:self.window.rootViewController];
		// Your code
	}

Ad shows as popup with video. You should request ad beforehand.
<!-- language: objective-c -->
 	- (void)viewWillAppear:(BOOL)animated {
		[super viewWillAppear:animated];
 
		// Register yourself as a delegate for ad callbacks
		[[Seedr instance] setDelegate:self];
 
		// Fetch ads early when a later display is likely. For 
		// example, at the beginning of a level. 
		[[Seedr instance] requestAdForSpace:@”SEEDR_DEFAULTSPACE”];
	}

You will be notified when ad is ready by calling delegate meth
<!-- language: objective-c -->
	- (void)onReceivedAdForSpace:(NSString*)space;


Invoke a takeover at a natural pause in your app. For example, when a level is completed, an article is read or a button is pressed.
<!-- language: objective-c -->
	- (void)showAd
	{
		// Check if ad is ready. If so, display the ad
    		if ([[Seedr instance] isAdAvailableForSpace:SEEDR_DEFAULTSPACE])
    		{
        		[[Seedr instance] showAdForSpace:SEEDR_DEFAULTSPACE presentType:SeedrPresentTypeCustom];
    		}
	}

Use callbacks to track app state.
<!-- language: objective-c -->
	// use it for pause your app
	- (void)adWillPresent:(NSString *)space;
	// use it for resume your app
	- (void)adDidDismiss:(NSString *)space;
	// use it for reward user after playing video
	- (void)rewardForSpace:(NSString *)space;