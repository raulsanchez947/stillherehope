# Quest Build Slice Changelog

## This slice

- Removed the runtime trainer-party species rewrite that was mutating scripted boss teams into baby or middle forms.
- Fixed Paul's standalone trainer presentation to read like a rival battle instead of a school-kid placeholder.
- Rebuilt May's Rustboro, Route 110, Route 119, and Lilycove checkpoint teams around a more deliberate Uxie-centered progression.
- Marked TM Case as legacy-only in item text so old saves do not pretend it is still the active TM container.
- Added a regional-evolution guide based on the live evolution table.
- Replaced Scott's visible overworld presence with Brendan across the former Scott route while preserving internal Scott flags/vars for save compatibility.
- Updated former Scott dialogue headers to Brendan and documented Brendan's Petalburg-to-League Champion reveal arc in the boss atlas.
- Added Brendan's Mauville 6v6 observer battle and hard-mode observer team to the live atlas.

## QA focus

- Check that Juan, Flannery, and other scripted bosses now use the exact species from their party tables.
- Check Paul's intro presentation in a direct battle slot.
- Check May's four major checkpoint battles for team variety and pressure.
- Check older saves that may still hold a TM Case item and confirm the description points players toward the TM pocket instead of implying the case is functional.
- Check former Scott scenes in Petalburg, Slateport, Route 119, Mossdeep, Ever Grande, S.S. Tidal, and Battle Frontier for Brendan sprite/text consistency.
