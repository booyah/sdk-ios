PlayHaven SDK for iOS User and Reference Guide
==============================================

This guide has the following sections:

* [Overview](#overview)
* [Version History](#version-history)
* [Integration](#integration)
* [API Reference](#api-reference)
* [Integration Test Console](#integration-test-console-overview)

Overview
========

PlayHaven is a mobile game LTV-maximization platform to help you take control of the business of your games.

Acquire, retain, re-engage, and monetize your players with the help of PlayHaven's powerful marketing platform. Integrate once and embrace the flexibility of the web as you build, schedule, deploy, and analyze your in-game promotions and monetization in real-time through PlayHaven's easy-to-use, web-based dashboard.

An API token and secret is required to use this SDK. These tokens uniquely identify your app to PlayHaven and prevent others from making requests to the API on your behalf. To get a token and secret, please visit the [PlayHaven dashboard](https://dashboard.playhaven.com).

For more information, see the [Help Center](http://help.playhaven.com) or contact us at [support@playhaven.com](mailto:support@playhaven.com).  We also recommend reviewing our [Optimization Guides](http://help.playhaven.com/customer/portal/topics/113947-optimization-guides/articles) to learn the best practices and get the most out of your PlayHaven integration. Additionally, the [Integration Test Console](#integration-test-console-overview) can help you test your integration against our APIs.

Also see the [Integration](#integration) and [Using the SDK](#using-the-sdk) sections in this document.

Version History
===============

1.13.2
======
* Addresses an issue which causes content units to not work on iOS 4.3
* Addresses an issue with the error handling of PHAPIRequest.m

1.13.1
======
* MAC Address, ODIN1, customUDID parameters are now sent on all requests
* The customUDID parameter now strips RFC 3986 Reserved Characters
* More unit tests added and a warning fixed

1.13.0
======
* UDID collection has been removed to comply with Apple’s policy for the use of device information, beginning May 1, 2013
* Receipt verification available on the SDK with server-side component available soon. See [Tracking in-app purchases](#tracking-in-app-purchases) for information on how to integrate this into your app
* Miscellaneous bug fixes

1.12.1
======
* iOS 6 compatibility improvements
* In-App iTunes purchases support for content units. See [Links to the App Store](#links-to-the-app-store)
* Fixes for crashes affecting devices running iOS versions lower than 5.0

1.12.0
======
* The SDK now automatically records the number of game sessions and the length of game sessions. This depends on a proper open request implementation. See [Recording game opens](#recording-game-opens)

1.11.0
======
* App Store launches now properly preserve affiliate link tokens
* Build settings changed to remove THUMB instructions from static library builds. This change only affects publishers using this SDK as a static library from the Unity plugin

1.10.4
======
* In-App Purchase (IAP) tracking requests now report accurate price information

1.10.3
======
* DNS resolution for API servers happens in a background thread

1.10.2
======
* Bugfixes for issues with canceling requests and a rare crash involving precaching

1.10.1
======
* Ability to opt out of user data collection at runtime

1.10.0
======
* In-App Purchase tracking and virtual goods promotion support. See [Triggering in-app purchases](#triggering-in-app-purchases) and [Tracking in-app purchases](#tracking-in-app-purchases) for information on how to integrate this into your app
* New documentation on how to disable Store Kit-based features in the SDK

Dashboard Setup
===========
Setting up PlayHaven is a two-step process: First, you set up your account, game, placements, and content units on the [PlayHaven Publisher Dashboard](https://dashboard.playhaven.com/). Then, you integrate the PlayHaven SDK into your application and add the necessary PlayHaven calls to your code. 

1. To set up your account, go to the [signup page](http://www.playhaven.com/developers) and sign up for a free PlayHaven developer account.
1. To set up your game, follow [these instructions](http://help.playhaven.com/customer/portal/articles/585190-how-to-add-a-game).
1. Placements are fundamental to how PlayHaven operates. You can think of them as billboards in your game: what appears on these billboards is controlled by what content units are attached to them. If no content unit is attached to them, nothing is shown and the user is not even aware that they are there. You can switch which content units are connected to which placements with the Publisher Dashboard. To set up your placements, follow [these instructions](http://help.playhaven.com/customer/portal/articles/585198-how-to-add-placements). For more information on placements, you can read [this article](http://help.playhaven.com/customer/portal/articles/243504-placements-overview).  
1. Set up a content unit. If this is your first time integrating PlayHaven, a good content unit to try first is an interstitial ad. Follow [these steps](http://help.playhaven.com/customer/portal/articles/585250-how-to-create-an-interstitial-ad) to set up a PlayHaven interstitial ad. Be sure to associate this content unit with one of the placements you created in the previous step (by checking the box next to the name of the placement). 


Integration
===========
If you are using Unity for your game, please integrate the [Unity SDK](https://github.com/playhaven/sdk-unity/) instead of the iOS SDK.

1. Add the following from the sdk-ios directory that you downloaded or cloned from github to your project:
    * src directory
    * Cache directory
1. Unless you are already using SBJSON, also add the following to your project:
    * JSON directory.
  If your project is already using SBJSON, then you may continue to use those classes or exchange them for the classes included with this SDK. Multiple copies of these classes in the same project may cause errors at compile time.
1. Ensure the following frameworks are included with your project. Add any missing frameworks in the Build Phases tab for your application's target:
    * UIKit.framework
    * Foundation.framework
    * CoreGraphics.framework
    * SystemConfiguration.framework
    * CFNetwork.framework
    * AdSupport.framework
    * StoreKit.framework (see next)
1. (optional) Most applications integrating PlayHaven SDK are already linked against StoreKit.framework. If your app does not use StoreKit.framework and you do not plan to use Virtual Goods Promotions or In-App Purchase Tracking features in your application, its is possible to disable SDK part supporting those features by setting the following preproccessor macro in your project or target's Build Settings:

		PH_USE_STOREKIT=0

    This makes it possible to build the SDK without Store Kit linked to your project.
1. (optional) If your project needs to be compatible with iOS 5.1 - iOS 4.0, make sure to set "AdSupport.framework" to "Optional" in the Build Phases' Link Binary With Libraries section for your application's target. Also, versions of Xcode prior to version 4.5 do not include AdSupport.framework. If you are using a version of Xcode prior to version 4.5, you will need to disable references to this framework. To do this, set the following preprocessor macro in your project or target's Build Settings:

		PH_USE_AD_SUPPORT=0

1. Include the PlayHavenSDK headers in your code wherever you are using PlayHaven request classes:

		#import "PlayHavenSDK.h"
1. Send a game open each time a game session starts: when your game is first launched as well as each time it is foregrounded. See [Recording game opens](#recording-game-opens).
1. For each placement, you need to send a content request and implement content request delegate methods. See [Requesting content for your placements](#requesting-content-for-your-placements).
1. If you are planning on using a More Games Widget in your game, we recommend also implementing a notification view for any placements that use this widget. This can improve chart opens performance by up to 300%. See [Add a notification view (notifier badge)](#add-a-notification-view-notifier-badge).

Using the SDK
=============
### Device tracking
It is possible to track individual iOS devices using different device identifiers. The identifiers that PlayHaven relies on most are identifierForAdvertising (IDFA), device MAC address (MAC), and Open Device Identification Number (ODIN). Since IDFA is only available on iOS 6.0 and later, and a significant portion of the iOS market is still on 5.x, it is important that MAC and ODIN be available for PlayHaven to use. By default, `PH_USE_MAC_ADDRESS=1` is set, which sends the device's wifi MAC address and ODIN values for the current device with the open request. If you previously added the preprocessor macro `PH_USE_MAC_ADDRESS=0`, **you should remove it**.

PlayHaven no longer uses UDID, as Apple is no longer accepting apps which use it. 

As to the use of MAC address, starting from beta release of iOS 7, low-level networking APIs that used to return a device MAC address return the fixed value. As a result it cannot be used as a unique identifier any longer. To address that change, PlayHaven will be releasing a version of the iOS SDK which doesn't use MAC address before the public release of iOS 7.

#### User opt-out
To comply with Apple policies for the use of device information, we've provided a mechanism for your app to opt-out of collection of MAC addresses. To set the opt out status for your app, use the following method:

    [PHAPIRequest setOptOutStatus:(BOOL)yesOrNo];

You are responsible for providing an appropriate user interface for user opt-out. User data is sent by default.

### Recording game opens
Your application must report each time that it comes to the foreground. PlayHaven uses these events to measure the click-through rate of your content units to help optimize the performance of your implementation. This request is asynchronous and may run in the background while your game is loading.

Consider putting an open request in _both_ of these application delegate methods.

The first method records a game open when the application is first launched:

	- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions

The second method records a game open each time the app moves to the foreground after being launched:

	- (void)applicationWillEnterForeground:(UIApplication *)application

The game open request is sent using the following code:

	[[PHPublisherOpenRequest requestForApp:@"TOKEN" secret:@"SECRET"] send]

Where token and secret should be replaced with the token and the secret of your application, which are alpha-numerical strings of 32 character length. You can find the token and the secret of your application in the game settings in the PlayHaven dashboard.

If you are using an internal identifier to track individual devices in this game, you may use the customUDID parameter to pass this identifier along to PlayHaven. You can set this parameter on an open request:

    PHPublisherOpenRequest *request = [PHPublisherOpenRequest requestForApp:MYTOKEN secret:MYSECRET];
    request.customUDID = @"CUSTOM_UDID" //optional
    [request send];

Alternatively, you can set the parameter through the `PHAPIRequest` class method:

    [PHAPIRequest setCustomUDID:(NSString *)@"CUSTOM_UDID"];

This value is now sent on every request. Additionaly, any Reserved Characters as specified by RFC 3986 will be removed.


#### Precaching content templates
PlayHaven automatically downloads and stores a number of content templates after a successful `PHPublisherOpenRequest`. This happens automatically in the background after each open request, so there's no integration required to take advantage of this feature.

<a id="request_placements"></a>
### Requesting content for your placements
You request content for your app using your API token, secret, and a placement tag to identify the placement for which you are requesting content. Implement `PHPublisherContentRequestDelegate` methods to receive callbacks from this request. Refer to the following section as well as the `example/PublisherContentViewController.m` file for a sample implementation.

The listing below demonstrates a content request for the placement game_launch. Be sure to substitute your own app's token and secret:

	PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:@"TOKEN" secret:@"SECRET" placement:@"game_launch" delegate:self];
	request.showsOverlayImmediately = YES; //optional, see next.
	[request send];

The placement tags are set using the PlayHaven dashboard.

Optionally, you can show the loading overlay immediately by setting the request object's `showsOverlayImmediately` property to YES. This is useful if you would like keep users from interacting with your UI while the content is loading.

#### Preloading requests (optional)
To make content requests more responsive, you may choose to preload a content unit for a given placement. This starts a request for a content unit without displaying it, preserving the content unit until you call `- (void)send` on a  content request for the same placement in your app.

    [[PHPublisherContentRequest requestForApp:(NSString *)token secret:(NSString *)secret placement:(NSString *)placement delegate:(id)delegate] preload];

You may set a delegate for your preload if you would like to be informed when a content request is ready to display. See the next sections for more details.

Preloading only works for the next content request for a given placement. If you are showing the same placement multiple times in your app, you will need to make additional preload requests after displaying that placement's content unit for the first time.

#### Starting a content request
The request is about to attempt to get content from the PlayHaven API.

	- (void)requestWillGetContent:(PHPublisherContentRequest *)request;

#### Receiving content
The request received some valid content from the PlayHaven API. This will be the last delegate method a preloading request will receive, unless there is an error.

	- (void)requestDidGetContent:(PHPublisherContentRequest *)request;

#### Preparing to show a content view
If there is content for this placement, it is loaded at this point. An overlay view appears over your app and a spinner indicates that the content is loading. Depending on the transition type for your content, your view may or may not be visible at this time. If you haven't done this, you should mute any sounds and pause any animations in your app.

	- (void)request:(PHPublisherContentRequest *)request contentWillDisplay:(PHContent *)content;

#### Content view finished loading
The content has been successfully loaded and the user is now interacting with the downloaded content view.

	- (void)request:(PHPublisherContentRequest *)request contentDidDisplay:(PHContent *)content;

#### Content view dismissing
The content has successfully dismissed and control is being returned to your app. This can happen as a result of the user clicking on the close button or clicking on a link that will open outside of the app. You may restore sounds and animations at this point.

	- (void)request:(PHPublisherContentRequest *)request contentDidDismissWithType:(PHPublisherContentDismissType *)type;

Type may be one of the following constants:

* PHPublisherContentUnitTriggeredDismiss - A user or a content unit dismissed the content request.
* PHPublisherNativeCloseButtonTriggeredDismiss - The user used the native close button to dismiss the view.
* PHPublisherApplicationBackgroundTriggeredDismiss - For iOS 4.0 and higher only. The content unit was dismissed because the app was sent to the background.
* PHPublisherNoContentTriggeredDismiss - The content unit was dismissed because there was no content assigned to this placement ID.

#### Content request failing
If for any reason the content request does not successfully return some content to display or fails to load after the overlay view has appeared, the request stops and any visible overlays are removed.

	- (void)request:(PHPublisherContentRequest *)request didFailWithError:(NSError *)error;

NOTE: `- (void)request:contentDidFailWithError:` is deprecated in favor of `request:didFailWithError:`. Please update any previous uses of the this method accordingly.

### Canceling requests
You can cancel any API request at any time using the `- (void)cancel` method. This also cancels any open network connections and cleans up any views in the case of content requests. Canceled requests will not send any more messages to their delegates.

You can also cancel all open API requests for a given delegate. This can be useful if you are not keeping references to API request instances you may have created. As with the `- (void)cancel` method, canceled requests will not send any more messages to delegates. To cancel all requests:

    [PHAPIRequest cancelAllRequestsWithDelegate:(id)delegate];

###Customizing content display
####Replace close button graphics
Use the following request method to replace the close button image with something that more closely matches your app. Images will be scaled to a maximum size of 40x40.

	- (UIImage *)request:(PHPublisherContentRequest *)request closeButtonImageForControlState:(UIControlState)state content:(PHContent *)content;

### Unlocking rewards with the SDK
PlayHaven allows you to reward users with virtual currency, in-game items, or any other content within your game. If you have configured unlockable rewards for your content units, you will receive unlock events through a delegate method. It is important to handle these unlock events in every placement that has rewards configured.

	- (void)request:(PHPublisherContentRequest *)request unlockedReward:(PHReward *)reward;

The `PHReward` object passed through this method has the following helpful properties:

  * name - The name of your reward as configured on the dashboard.
  * quantity - An integer representing the quantity associated with the reward.
  * receipt - A unique identifier that is used to detect duplicate reward unlocks. Your app should ensure that each receipt is only unlocked once.

### Triggering in-app purchases
Using the Virtual Goods Promotion content unit, PlayHaven can be used to trigger In-App Purchase requests in your app using the following:

	- (void)request:(PHPublisherContentRequest *)request makePurchase:(PHPurchase *)purchase;

The `PHPurchase` object passed through this method has the following properties:

  * productIdentifier - The product identifier for your purchase.  This is a unique string used for Store Kit requests.
  * quantity - An integer representing the quantity associated with the purchase.
  * receipt - A unique identifier.

**Note:** You are responsible for making a SKProduct request before initiating the purchase of this item so as to comply with IAP requirements.

**Note:** In earlier versions of PlayHaven, you had to call `[purchase reportResolution:(PHPurchaseResolution)resolution]` to dismiss the Virtual Goods Promotion content unit. You no longer have to do this.   

### Links to the App Store
As of 1.12.1, links that open in the App Store will instead launch Apple's In-App iTunes view controller as a modal popup. This view controller is independent of any content request so you will not receive delegate events from it.

In-App iTunes purchases are like other in-app purchases in that when launched from an app that is being debugged (through XCode), or distributed using an Ad-Hoc profile, they interact with Apple's sandbox iTunes environment. Thus, purchases won't seem to work unless they're tested with a sandbox iTunes account, which is set up through iTunes Connect. The purchases will work as expected from a version of the app downloaded from the App Store, which is signed by Apple.

### Tracking in-app purchases
All in-app purchases made by users in your app, should be tracked by using PlayHaven's IAP tracking calls. You should do this for **ALL** purchases, whether these purchases were triggered by PlayHaven Virtual Goods Promotions or not. By providing data on your in-app purchases to PlayHaven, you can track your users' overall lifetime value as well as track conversions from your Virtual Goods Promotion content units. This is done using the `PHPublisherIAPTrackingRequest` class. To report successful purchases use the following either in your `SKPaymentQueueObserver` instance or after a purchase has been successfully delivered:

    PHPublisherIAPTrackingRequest *request = [PHPublisherIAPTrackingRequest requestForApp:TOKEN secret:SECRET product:PRODUCT_IDENTIFIER quantity:QUANTITY resolution:PHPurchaseResolutionBuy receiptData:RECEIPT_DATA];
    [request send];

Purchases that are canceled or encounter errors should be reported using the following:

    PHPublisherIAPTrackingRequest *request = [PHPublisherIAPTrackingRequest requestForApp:TOKEN secret:SECRET product:PRODUCT_IDENTIFIER quantity:QUANTITY error:ERROR_OBJECT receiptData:RECEIPT_DATA];
    [request send];

If the error comes from an `SKPaymentTransaction` instance's `error` property, the SDK will automatically select the correct resolution (buy/cancel) based on the `NSError` object that is passed in.

#### Receipt verification

Additionally, you can now send the receipt data to the PlayHaven server to verify purchases against Apple receipts to insure that only valid purchases are being reported. To verify that a purchase is valid, add the receipt data from the In-App Purchase to the IAP tracking request by using the `receiptData` argument to the method.

### Add a notification view (notifier badge)
Adding a notification view to your More Games button can increase the number of More Games Widget opens for your game by up to 300%. To create a notification view:

    PHNotificationView *notificationView = [[PHNotificationView alloc] initWithApp:MYTOKEN secret:MYSECRET placement:@"more_games"];
    [myView addSubview:notificationView];
    [notificationView release];

Add the notification view as a subview somewhere in your view controller's view. Notification views will remain anchored to the center of the position they are placed in the view, even as the size of the badge changes. You can adjust the position of the badge by setting the `center` property:

    notificationView.center = CGPointMake(10,10);

The notification view will query and update itself when its `- (void)refresh` method is called:

    [notificationView refresh];

We recommend refreshing the notification view each time it appears in your UI. See `examples/PublisherContentViewController.m` for an example.

You also need to clear any notification view instances when you successfully launch a content unit. You do this by using the `- (void)clear` method on any notification views that you wish to clear.

#### Testing PHNotificationView
Most of the time the API returns an empty response, which means a notification is not shown. You can see a sample notification by using `- (void)test;` wherever you would use `- (void)refresh`. It is listed as deprecated to remind you to switch all instances of `- (void)test` in your code to `- (void)refresh;`.

#### Customizing notification rendering with PHNotificationRenderer
`PHNotificationRenderer` is a base class that draws a notification view for the given notification data. The base class implements a blank notification view used for unknown notification types. `PHNotificationBadgeRenderer` renders an iOS default-style notification badge with a given value string. You can customize existing notification renderers and register new ones at runtime using the following method on `PHNotificationView`:

	+ (void)setRendererClass:(Class)class forType:(NSString *)type;

Your `PHNotificationRenderer` subclass needs to implement the following methods to draw and size your notification view appropriately:

	- (void)drawNotification:(NSDictionary *)notificationData inRect:(CGRect)rect;

This method is called inside of the `PHNotificationView` instance `- (void)drawRect:` method whenever the view needs to be drawn. You use specific keys inside of `notificationData` to draw your badge in the view. If you need access to the graphics context you may use the `UIGraphicsGetCurrentContext()` function.

	- (CGSize)sizeForNotification:(NSDictionary *)notificationData;

This method is called to calculate an appropriate frame for the notification badge each time the notification data changes. Using specific keys inside of `notificationData`, you need to calculate an appropriate size.

### Setting the plugin identifier (plugin creation only)
If you are creating a specific plugin (for Unity or AdobeAIR, e.g.) by wrapping the SDK in your code, you should set the SDK's plugin identifier to something meaningful (e.g., "com.unity.JohnDoe-v1.1.1"). Any Reserved Characters as specified by RFC 3986 will be removed and identifiers will be trimmed to 42 characters.

    [PHAPIRequest setPluginIdentifier:(NSString *)identifier];

Integration Test Console Overview
=================================

PlayHaven provides an [Integration Test Console](http://console.playhaven.com). It's located at http://console.playhaven.com

At the Testing Console, developers can enter their UDID. The Testing Console listens for events coming from the test device and displays a log of output, including successes, failures, and helpful information. The test device must be registered as test device from the PlayHaven Dashboard and "enabled" via the Publisher Dashboard.

Currently one cannot "export" their console log but you can copy and paste it into a text file or spreadsheet. To search, please use Command+F.

To begin, enter your UDID and follow the "Testing Instructions" in the light blue box to view events and comments.

The following can currently be checked:

* Upon Open Request:
  * Device ID (UDID)
  * Open requests sent
  * Token/secret present
  * SDK Version
  * IDFA sent (iOS only)
* Placements:
  * Placement detection (Content request)
  * Pre-loading (Pre-loading request)
* IAP:
  * Check that pricing is present (IAP transaction request)
