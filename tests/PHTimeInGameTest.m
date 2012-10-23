//
//  PHTimeInGameTest.m
//  playhaven-sdk-ios
//
//  Created by Thomas DiZoglio on 7/5/12.
//  Copyright (c) 2012 Play Haven. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>
#import <UIKit/UIKit.h>
#import "PHPublisherOpenRequest.h"
#import "PHTimeInGame.h"

@interface PHTimeInGameTest : SenTestCase
@end


@implementation PHTimeInGameTest

-(void)testTimeInGame{
    [[PHTimeInGame getInstance] gameSessionRestart];
    CFAbsoluteTime time = [[PHTimeInGame getInstance] getSumSessionDuration];
    STAssertTrue(time == 0, @"Session should be == 0 since reset. Value: %f", time);
    int lastCount = [[PHTimeInGame getInstance] getCountSessions];
    STAssertTrue(lastCount == 0, @"Session count should be 0 since reset. Value: %d", lastCount);
    
    [[PHTimeInGame getInstance] gameSessionStarted];
    int sessionCount = [[PHTimeInGame getInstance] getCountSessions];
    STAssertTrue(sessionCount == 1, @"Session count should be a value of 1. Value %d", sessionCount);

    CFAbsoluteTime diffTime = [[PHTimeInGame getInstance] getCurrentSessionDuration];
    STAssertTrue(diffTime > 0, @"Session MUST be greater than 0");

    [[PHTimeInGame getInstance] gameSessionStopped];
    int firstCount = [[PHTimeInGame getInstance] getCountSessions];
    STAssertTrue(firstCount == 1, @"Session count should be 1, value: %d", firstCount);

    [[PHTimeInGame getInstance] gameSessionStarted];
    sessionCount = [[PHTimeInGame getInstance] getCountSessions];
    STAssertTrue(sessionCount == 2, @"Session count should be a value of 2. value: %d", sessionCount);
}

@end