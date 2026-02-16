// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingTitleScreen.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UBowlingTitleScreen::OnStartGame(int numPlayers)
{
	UGameplayStatics::OpenLevel(GetWorld(), GameplayMapUrl);
}

void UBowlingTitleScreen::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#endif

void UBowlingTitleScreen::OnHapticTest()
{
#if PLATFORM_ANDROID
	    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        jobject Activity = FAndroidApplication::GetGameActivityThis();
        if (Activity == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("No activity found"));
            return;
        }

        jclass ActivityClass = Env->GetObjectClass(Activity);

        // Get the current View from activity: call getWindow().getDecorView()
        jmethodID GetWindowMethod = Env->GetMethodID(ActivityClass, "getWindow", "()Landroid/view/Window;");
        jobject Window = Env->CallObjectMethod(Activity, GetWindowMethod);
        if (Window == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("No window found"));
            return;
        }

        jclass WindowClass = Env->GetObjectClass(Window);
        jmethodID GetDecorViewMethod = Env->GetMethodID(WindowClass, "getDecorView", "()Landroid/view/View;");
        jobject View = Env->CallObjectMethod(Window, GetDecorViewMethod);

        if (View == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("No decor view found"));
            return;
        }
        jclass ViewClass = Env->GetObjectClass(View);
        
        // Haptic feedback constant - example CONTEXT_CLICK = 1
        jint feedbackConstant = 1; 
        
        // Get method ID for performHapticFeedback
        jmethodID PerformHapticFeedbackMethod = Env->GetMethodID(ViewClass, "performHapticFeedback", "(I)Z");
        if (PerformHapticFeedbackMethod == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Method performHapticFeedback not found!"));
            return;
        }

        jboolean Result = Env->CallBooleanMethod(View, PerformHapticFeedbackMethod, feedbackConstant);

        // Optional: check Result (true if performed)
        UE_LOG(LogTemp, Log, TEXT("performHapticFeedback returned: %d"), Result);

        // Clean up local refs
        Env->DeleteLocalRef(ViewClass);
        Env->DeleteLocalRef(View);
        Env->DeleteLocalRef(WindowClass);
        Env->DeleteLocalRef(Window);
        Env->DeleteLocalRef(ActivityClass);
    }
#else
    UE_LOG(LogTemp, Warning, TEXT("Haptic feedback not supported on this platform"));
#endif

}

