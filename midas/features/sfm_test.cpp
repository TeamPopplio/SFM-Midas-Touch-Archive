#include "stdafx.h"
#include "..\feature.hpp"
#include <QtCore>
#include <QtGui>
#include <QMetaObject>
#include <SDK/swarm/public/tier0/dbg.h>

typedef void* (__fastcall* _MainWindowInteracted)(void* thisptr, int edx);

// Test for SFM functionality.
class SFMTest : public FeatureWrapper<SFMTest>
{
public:
	static void* __fastcall HOOKED_SFMLoaded(void* thisptr, int edx);

	// Error flags:
	// 1 - Animation set editor not found.
	int AcknowledgedErrorFlags;

	int AnimationSetEditorMaxAttempts = 100;
	int AnimationSetEditorAttempts = AnimationSetEditorMaxAttempts;

	QWidget* MainWindow;
	QWidget* AnimationSetEditor;

protected:
	_MainWindowInteracted ORIG_SFMLoaded = nullptr;

	virtual bool ShouldLoadFeature() override
	{
		return true;
	}

	virtual void InitHooks() override
	{
		HOOK_FUNCTION(ifm, SFMLoaded);
	}

	virtual void LoadFeature() override
	{
		
	}

	virtual void UnloadFeature() override
	{

	}
};

static SFMTest sfm_test;

void* __fastcall SFMTest::HOOKED_SFMLoaded(void* thisptr, int edx)
{
	if (sfm_test.MainWindow == nullptr)
	{
		foreach(QWidget * widget, qApp->topLevelWidgets())
		{
			if (widget->inherits("QMainWindow"))
			{
				sfm_test.MainWindow = widget;
				Msg("Midas Touch: Found main window.\n");
				break;
			}
		}
	}
	if (sfm_test.MainWindow != nullptr)
	{
		// Replace title of SFM.
		QString windowTitle = sfm_test.MainWindow->windowTitle();
		QString replaceString = QString::fromAscii("Source Filmmaker [Beta]");
		QString afterString = QString::fromAscii("SFM: Midas Touch");
		sfm_test.MainWindow->setWindowTitle(windowTitle.replace(replaceString, afterString));
		// Find animation set editor.
		if (sfm_test.AnimationSetEditor == nullptr)
		{
			QWidget* widget = sfm_test.MainWindow->findChild<QWidget*>(QString::fromAscii("QAnimationSetEditor"));
			if (widget != nullptr)
			{
				Msg("Midas Touch: Found animation set editor with %d/%d attempts remaining.\n", sfm_test.AnimationSetEditorAttempts, sfm_test.AnimationSetEditorMaxAttempts);
				sfm_test.AnimationSetEditor = widget;
			}
			else if (sfm_test.AnimationSetEditorAttempts > 0)
			{
				sfm_test.AnimationSetEditorAttempts--;
			}
			else if ((sfm_test.AcknowledgedErrorFlags & 1) == 0)
			{
				// Show error modal.
				Msg("Midas Touch: Failed to find animation set editor within %d attempts.\n", sfm_test.AnimationSetEditorMaxAttempts);
				sfm_test.AcknowledgedErrorFlags = 1 >> sfm_test.AcknowledgedErrorFlags;
				QMessageBox messageBox;
				messageBox.critical(0, "Midas Touch: Error", "Could not find the animation set editor widget!\nMidas Touch may not function correctly.\nPlease open the animation set editor widget.");
			}
		}
	}
	return sfm_test.ORIG_SFMLoaded(thisptr, edx);
}

QWidget* GetMainWindow()
{
	return sfm_test.MainWindow;
}

QWidget* GetAnimationSetEditor()
{
	return sfm_test.AnimationSetEditor;
}
