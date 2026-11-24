import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import status as status_report


class ImplementedNonmatchingReportTests(unittest.TestCase):
    def test_removes_matching_and_not_started_history_but_keeps_overrides(self):
        rows = [
            {
                "name": "func_00100000",
                "status": "MATCHING",
                "source_edited_at": "",
            },
            {
                "name": "func_00100008",
                "status": "NOT STARTED",
                "source_edited_at": "",
            },
            {
                "name": "func_00100018",
                "status": "MATCHING",
                "source_edited_at": "2026-11-23T19:00:00Z",
            },
            {
                "name": "func_00100020",
                "status": "NOT MATCHING",
                "source_edited_at": "2026-11-23T21:00:00Z",
            },
            {
                "name": "func_00100028",
                "status": "NOT STARTED",
                "source_edited_at": "",
            },
            {
                "name": "func_00100030",
                "status": "NOT MATCHING",
                "source_edited_at": "2026-11-23T22:00:00Z",
            },
        ]

        with tempfile.TemporaryDirectory() as temporary_directory:
            report_path = Path(temporary_directory) / "implemented-nonmatching.txt"
            report_path.write_text(
                "func_00100000\n"
                "------- CHECKPOINT\n"
                "func_00100008\n"
                "func_00100018\n"
                "func_00100020\n"
                "func_00100028\n",
                encoding="utf-8",
            )
            with patch.object(
                status_report, "IMPLEMENTED_NONMATCHING_REPORT", report_path
            ):
                status_report.update_implemented_nonmatching_report(
                    rows,
                    {"func_00100018"},
                )

            self.assertEqual(
                report_path.read_text(encoding="utf-8").splitlines(),
                [
                    "func_00100030",
                    "------- CHECKPOINT",
                    "func_00100018",
                    "func_00100020",
                ],
            )


if __name__ == "__main__":
    unittest.main()
